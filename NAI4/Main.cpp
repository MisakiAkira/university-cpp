#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <functional>

using namespace std;

struct DataPoint {
	vector<double> attributes;
	int cluster;

	bool operator==(const DataPoint& other) const {
		return (attributes == other.attributes) && (cluster == other.cluster);
	}
};

vector<DataPoint> readDataPoints(string filename) {
	vector<DataPoint> dataPoints;
	ifstream file(filename);
	string line;
	while (getline(file, line)) {
		DataPoint dataPoint;
		double attribute;
		char delimiter;
		istringstream iss(line);
		while (iss >> attribute) {
			dataPoint.attributes.push_back(attribute);
			if (iss.peek() == ',') {
				iss.ignore();
			}
			else {
				getline(iss, line);
				delimiter = line[0];
			}
		}
		dataPoint.cluster = -1;
		dataPoints.push_back(dataPoint);
	}
	return dataPoints;
}

double euclideanDistance(const DataPoint& a, const DataPoint& b) {
	double distance = 0.0;
	for (int i = 0; i < b.attributes.size(); i++) {
		distance += pow(a.attributes[i] - b.attributes[i], 2);
	}
	return sqrt(distance);
}

vector<DataPoint> initializeClusters(const vector<DataPoint>& dataPoints, int k) {
	vector<DataPoint> clusters;
	for (int i = 0; i < k; i++) {
		clusters.push_back(dataPoints[i]);
		clusters[i].cluster = i;
	}
	return clusters;
}

void assignDataPointsToClusters(vector<DataPoint>& dataPoints, const vector<DataPoint>& clusters) {
	for (auto& dataPoint : dataPoints) {
		double minDistance = INFINITY;
		for (const auto& cluster : clusters) {
			double distance = euclideanDistance(dataPoint, cluster);
			if (distance < minDistance) {
				minDistance = distance;
				dataPoint.cluster = cluster.cluster;
			}
		}
	}
}

vector<DataPoint> computeCentroids(const vector<DataPoint>& dataPoints, int k) {
	vector<DataPoint> centroids(k);
	vector<int> counts(k, 0);
	for (const auto& dataPoint : dataPoints) {
		int cluster = dataPoint.cluster;
		centroids[cluster].attributes.resize(dataPoint.attributes.size());
		counts[cluster]++;
		for (int i = 0; i < dataPoint.attributes.size(); i++) {
			centroids[cluster].attributes[i] += dataPoint.attributes[i];
		}
	}
	for (int i = 0; i < k; i++) {
		centroids[i].cluster = i;
		// if dobavlen mnoi
		if (counts[i] == 0) {
			int index = dataPoints.size() - 1 - i;
			centroids[i].attributes = dataPoints[index].attributes;
			continue;
		}
		for (int j = 0; j < centroids[i].attributes.size(); j++) {
			centroids[i].attributes[j] /= counts[i];
		}
	}
	return centroids;
}

double computeSumOfSquaredDistances(const vector<DataPoint>& dataPoints, const vector<DataPoint>& clusters) {
	double sum = 0.0;
	for (const auto& dataPoint : dataPoints) {
		int cluster = dataPoint.cluster;
		double distance = euclideanDistance(dataPoint, clusters[cluster]);
		sum += pow(distance, 2);
	}
	return sum;
}

void displayClusterSizes(const vector<DataPoint>& dataPoints, int k) {
	vector<int> clusterSizes(k);
	for (const auto& dataPoint : dataPoints) {
		clusterSizes[dataPoint.cluster]++;
	}
	cout << "Cluster sizes: ";
	for (int i = 0; i < k; i++) {
		cout << clusterSizes[i];
		if (i < k - 1) {
			cout << ", ";
		}
	}
	cout << endl;
}

void displayClusterInfo(const vector<DataPoint>& dataPoints, const vector<DataPoint>& clusters, int iteration) {
	cout << "Iteration " << iteration << endl;
	displayClusterSizes(dataPoints, clusters.size());
	cout << "Sum of squared distances within clusters: " << computeSumOfSquaredDistances(dataPoints, clusters) << endl;
	cout << endl;
}

void kMeansClustering(vector<DataPoint>& dataPoints, int k) {
	vector<DataPoint> clusters = initializeClusters(dataPoints, k);
	int iteration = 0;
	bool changed = true;
	while (changed) {
		iteration++;
		assignDataPointsToClusters(dataPoints, clusters);
		vector<DataPoint> newClusters = computeCentroids(dataPoints, k);
		if (newClusters == clusters) {
			changed = false;
		}
		else {
			clusters = newClusters;
		}
		displayClusterInfo(dataPoints, clusters, iteration);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cout << "Usage: kmeans <filename> <k>" << endl;
		return 1;
	}
	string filename = argv[1];
	int k = stoi(argv[2]);
	vector<DataPoint> dataPoints = readDataPoints(filename);
	kMeansClustering(dataPoints, k);
	return 0;
}