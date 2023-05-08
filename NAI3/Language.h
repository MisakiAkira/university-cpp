#pragma once
#include <string>
#include <algorithm>
#include <random>

class Language
{
private:
	double m_preceptron[27] = { 0 };
	double m_alpha;
	std::string m_name;
	static void textToDouble(double (&doubles)[26], std::string const& text)
	{
		std::fill(doubles, doubles + 26, 0.0);
		int count = 0;

		for (auto ch : text)
		{
			ch = tolower(ch);
			if (ch < 'a' || ch > 'z')
			{
				continue;
			}
			doubles[ch - 'a']++;
			count++;
		}
		for (int i = 0; i < (sizeof(doubles) / sizeof(doubles[0])) - 1; i++)
		{
			doubles[i] /= count;
		}
	}
public:
	Language(std::string const& name, double alpha) : m_name(name), m_alpha(alpha)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> distr(0.0, 1.0);

		for (int i = 0; i < 27; ++i)
		{
			m_preceptron[i] = distr(gen);
			//m_preceptron[i] = 0;
		}
	}

	Language(const Language& other) : m_alpha(other.m_alpha), m_name(other.m_name) {
		std::copy(other.m_preceptron, other.m_preceptron + 27, m_preceptron);
	}
	Language& operator=(const Language& other) {
		if (this != &other) {
			m_alpha = other.m_alpha;
			m_name = other.m_name;
			std::copy(other.m_preceptron, other.m_preceptron + 27, m_preceptron);
		}
		return *this;
	}

	bool check(double const (&doubles)[26])
	{
		double sum = 0;
		for (int i = 0; i < ((sizeof(doubles) / sizeof(doubles[0])) - 1); i++)
		{
			sum += doubles[i] * m_preceptron[i];
		}
		return sum >= m_preceptron[(sizeof(m_preceptron) / sizeof(m_preceptron[0])) - 1];
	}
	bool check(std::string const& text)
	{
		double doubles[26];
		textToDouble(doubles, text);
		return check(doubles);
	}
	double checkWithReturn(double const (&doubles)[26])
	{
		double sum = 0;
		for (int i = 0; i < ((sizeof(doubles) / sizeof(doubles[0])) - 1); i++)
		{
			sum += doubles[i] * m_preceptron[i];
		}
		return m_preceptron[((sizeof(m_preceptron)/sizeof(m_preceptron[0])) - 1)] - sum;
	}
	double checkWithReturn(std::string const& text)
	{
		double doubles[26];
		textToDouble(doubles, text);
		return checkWithReturn(doubles);
	}
	void train(std::string const& text, bool currect)
	{
		double doubles[26];
		double tmp[27] = { 0 };
		textToDouble(doubles, text);
		for (int i = 0; i < sizeof(doubles) / sizeof(doubles[0]); i++)
		{
			tmp[i] = m_preceptron[i] + (currect ? 1 : -1) * m_alpha * doubles[i];
		}
		tmp[(sizeof(tmp) / sizeof(tmp[0])) - 1] = m_preceptron[(sizeof(m_preceptron) / sizeof(m_preceptron[0])) - 1] + (currect ? 1 : -1) * m_alpha;
		std::copy(tmp, tmp + (sizeof(tmp) / sizeof(tmp[0])-1), m_preceptron);
	}
	double* getPreceptron() { return m_preceptron; }
	std::string& getName() { return m_name; };
};