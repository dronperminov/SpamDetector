#include <iostream>
#include <string>
#include <vector>

#include "SpamDetector.hpp"
#include "TextReader.hpp"

using namespace std;

void Evaluate(const string& trainPath, const string& testPath) {
	SpamDetector detector(trainPath);

	TextReader reader;
	vector<Text> texts = reader.ReadTexts(testPath);

	int correct = 0;

	double trueSpam = 0;
	double trueHam = 0;
	double falseSpam = 0;
	double falseHam = 0;

	for (int i = 0; i < texts.size(); i++) {
		bool isSpam = detector.DetectSpam(texts[i].tokens);

		if (isSpam && texts[i].label == "spam")
			trueSpam++;

		if (!isSpam && texts[i].label == "ham")
			trueHam++;

		if (isSpam && texts[i].label == "ham")
			falseSpam++;

		if (!isSpam && texts[i].label == "spam")
			falseHam++;

		if ((texts[i].label == "spam" && isSpam) || (texts[i].label == "ham" && !isSpam))
			correct++;
	}

	double precisionSpam = trueSpam / (trueSpam + falseSpam);
	double recallSpam = trueSpam / (trueSpam + falseHam);
	double f1Spam = 2 * precisionSpam * recallSpam / (precisionSpam + recallSpam);

	double precisionHam = trueHam / (trueHam + falseHam);
	double recallHam = trueHam / (trueHam + falseSpam);
	double f1Ham = 2 * precisionHam * recallHam / (precisionHam + recallHam);

	cout << "Accuracy: " << (correct * 100.0 / texts.size()) << "%" << endl;
	cout << "Spam: precision: " << precisionSpam << ", recall: " << recallSpam << ", f1: " << f1Spam << endl;
	cout << " Ham: precision: " << precisionHam << ", recall: " << recallHam << ", f1: " << f1Ham << endl;
	cout << "F1: " << (f1Spam + f1Ham) / 2 << endl;
}

int main() {
	setlocale(LC_ALL, "russian");

	Evaluate("data/train.csv", "data/test.csv");

	SpamDetector detector("data/all.csv");
	detector.ShowStat();

	cout << ">";
	string text;
	while (getline(cin, text) && text != "") {
		Tokenizer tokenizer;
		detector.DetectSpam(tokenizer.Tokenize(text), true);
		cout << ">";
	}
}