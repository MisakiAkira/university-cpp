#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "Language.h"

namespace fs = std::filesystem;

bool iequals(const std::string& a, const std::string& b)
{
	return std::equal(a.begin(), a.end(),
		b.begin(), b.end(),
		[](char a, char b) {
			return tolower(a) == tolower(b);
		});
}

void overAllTrain(std::vector<Language>& languages, fs::path const& path)
{
	bool wasCalled;
	do
	{
		wasCalled = false;
		for (auto const& language : fs::directory_iterator(path))
		{
			if (!fs::is_directory(language))
			{
				continue;
			}

			for (auto& lang : languages)
			{
				for (auto const& file : fs::directory_iterator(language))
				{
					if (fs::path(file).extension() != ".txt")
					{
						continue;
					}
					std::fstream fileReader;
					fileReader.open(file.path().c_str(), std::ios::in);
					if (fileReader.is_open())
					{
						std::string text = "";
						std::string line;
						while (std::getline(fileReader, line))
						{
							text += line;
						}


						if (lang.check(text) != (fs::path(language).filename().string() == lang.getName()))
						{
							std::cout << "Training: " << lang.getName() << "\t on: " << fs::path(language).filename().string() << '\n';
							lang.train(text, iequals(fs::path(language).filename().string(), lang.getName()));
							wasCalled = true;
						}
					}

				}
			}
		}
	} while (wasCalled);
	return;
}

void initLanguages(std::vector<Language>& languages, fs::path const& path, double& alpha)
{
	for (auto const& language : fs::directory_iterator(path))
	{
		if (!fs::is_directory(language))
		{
			continue;
		}
		languages.push_back(Language(fs::path(language).filename().string(), alpha));

		for (auto const& file : fs::directory_iterator(language))
		{
			if (fs::path(file).extension() != ".txt")
			{
				continue;
			}
			std::fstream fileReader;
			fileReader.open(file.path().c_str(), std::ios::in);
			if (fileReader.is_open())
			{
				std::string text = "";
				std::string line;
				while (std::getline(fileReader, line))
				{
					text += line;
				}

				bool wasCalled;
				do
				{
					wasCalled = false;
					for (auto& lang : languages)
					{
						if (lang.check(text) != (fs::path(language).filename().string() == lang.getName()))
						{
							lang.train(text, fs::path(language).filename().string() == lang.getName());
							wasCalled = true;
						}
					}
				} while (wasCalled);
			}
		}
	}
	return;
}

int main()
{
	fs::path path = fs::current_path().append("Languages");
	if (!fs::exists(path) || !fs::is_directory(path))
	{
		std::cout << "The path doesn't exist" << '\n';
		return 0;
	}

	std::vector<Language> languages;
	std::string tmp;

	std::cout << "Enter alpha" << '\n';
	std::getline(std::cin, tmp);
	double alpha = std::stod(tmp);

	initLanguages(languages, path, alpha);
	overAllTrain(languages, path);

	//Uncoment to check double[] insides
	/*double doubles[26]{ 0 };
	for (auto& language : languages)
	{
		double* doubles = language.getPreceptron();
		std::cout << "Language: " << language.getName() << std::endl;
		for (int i = 0; i < 26; i++)
		{
			std::cout << doubles[i] << " ";
		}
		std::cout << std::endl;
	}*/


	std::string input;
	std::cout << "Enter text\n";
	std::getline(std::cin, input);
	while (!iequals(input, "exit"))
	{
		bool recognized = false;
		for (auto& lang : languages)
		{
			if (lang.check(input))
			{
				std::cout << "Languae: " << lang.getName() << '\n';
				recognized = true;
				break;
			}
		}
		if (!recognized)
		{
			std::string last = languages.at(0).getName();
			double min = languages.at(0).checkWithReturn(input);
			for (auto& lang : languages)
			{
				if (lang.checkWithReturn(input) < min)
				{
					last = lang.getName();
					min = lang.checkWithReturn(input);
				}
			}
			std::cout << "Language: " << last << '\n';
		}
		std::cout << "\nEnter text\n";
		std::getline(std::cin, input);
	}
	return 0;
}