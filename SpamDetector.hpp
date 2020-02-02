#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#include "TextReader.hpp"

using namespace std;

class SpamDetector {
	const string SPAM = "spam";
	const string HAM = "ham";

	struct ClassInfo {
		int textCount; // ���������� �������
		int wordsCount; // ����� ���������� ����
		map<string, int> dictionary; // ������� ���������� ����
	};

	vector<Text> texts;
	ClassInfo spam;
	ClassInfo ham;

	map<string, int> allDictionary;

	map<string, int> GetDictionaryByLabel(const string &label = ""); // ��������� ������� ���������� ���� � �� ����������� � �������
	double EvaluateLabel(const vector<string> &tokens, const string& label); // ��������� ������ ��� �������� �����

public:
	SpamDetector(const string &path);

	void ShowStat(); // ����������� ����������
	bool DetectSpam(const vector<string> &tokens, bool verbose = false); // ����������� �����
};

SpamDetector::SpamDetector(const string &path) {
	TextReader reader;
	texts = reader.ReadTexts(path);

	spam.textCount = 0;
	spam.wordsCount = 0;

	ham.textCount = 0;
	ham.wordsCount = 0;
	
	for (int id = 0; id < texts.size(); id++) {
		if (texts[id].label == SPAM) {
			spam.textCount++;
			spam.wordsCount += texts[id].tokens.size();
		}
		else if (texts[id].label == HAM) {
			ham.textCount++;
			ham.wordsCount += texts[id].tokens.size();
		}
	}

	allDictionary = GetDictionaryByLabel();
	spam.dictionary = GetDictionaryByLabel(SPAM);
	ham.dictionary = GetDictionaryByLabel(HAM);
}

// ��������� ������� ���������� ���� � �� ����������� � �������
map<string, int> SpamDetector::GetDictionaryByLabel(const string &label) {
	map<string, int> dictionary;

	for (int i = 0; i < texts.size(); i++) {
		if (label != "" && texts[i].label != label) // ���� ����� ������, �� �� ������������� ������
			continue; // ���������� �

		for (int j = 0; j < texts[i].tokens.size(); j++) {
			string token = texts[i].tokens[j]; // �������� �����

			if (dictionary.find(token) == dictionary.end()) // ���� ������ ��� � �������
				dictionary[token] = 1; // �� ��������� ���
			else
				dictionary[token] += 1; // ����� ����������� ��� �������
		}
	}
	
	return dictionary; // ���������� �������
}

// ����������� ����������
void SpamDetector::ShowStat() {
	cout << "total texts: " << texts.size() << endl;
	cout << "  ham texts: " << ham.textCount << endl;
	cout << " spam texts: " << spam.textCount << endl;
	cout << endl;
	cout << "total unique words: " << allDictionary.size() << endl;
	cout << " spam unique words: " << spam.dictionary.size() << endl;
	cout << "  ham unique words: " << ham.dictionary.size() << endl;
	cout << endl;
	cout << "spam total words: " << spam.wordsCount << endl;
	cout << " ham total words: " << ham.wordsCount << endl;
}

// ��������� ������ ��� �������� �����
double SpamDetector::EvaluateLabel(const vector<string> &tokens, const string& label) {
	ClassInfo info = label == SPAM ? spam : ham;
	double score = log((double)info.textCount / texts.size());

	for (int i = 0; i < tokens.size(); i++) {
		string token = tokens[i];
		double Wc = info.dictionary.find(token) == info.dictionary.end() ? 0 : info.dictionary[token];
		score += log((Wc + 1) / (info.wordsCount + allDictionary.size()));
	}

	return score;
}

// ����������� �����
bool SpamDetector::DetectSpam(const vector<string> &tokens, bool verbose) {
	double spamScore = EvaluateLabel(tokens, SPAM);
	double hamScore = EvaluateLabel(tokens, HAM);

	double spamExp = exp(spamScore);
	double hamExp = exp(hamScore);
	double totalExp = spamExp + hamExp;

	// �������� �����������
	double spamP = spamExp / totalExp;
	double hamP = hamExp / totalExp;

	// �������, ���� ����� ������
	if (verbose) {
		cout << "spam score: " << spamScore << ", probability: " << spamP << endl;
		cout << " ham score: " << hamScore << ", probability: " << hamP << endl;
	}

	return spamScore > hamScore; // ������� ������, ���� ������ ����� ����
}