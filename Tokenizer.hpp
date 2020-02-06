#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Tokenizer {
	set<string> stopWords; // список стоп слов

	bool IsLetter(char c) const; // проверка символа на букву
	bool IsDigit(char c) const; // проверка символа на цифру
	bool IsLetterOrDigit(char c) const; // проверка символа на букву или цифру
	char ToLower(char c) const; // перевод символа в нижний регистр

	vector<string> RemoveStopWords(vector<string> &tokens); // удаление стоп слов
public:
	Tokenizer();
	vector<string> Tokenize(istream& f, bool removeStopWords = true);
	vector<string> Tokenize(const string& s, bool removeStopWords = true);
};

// проверка символа на букву
bool Tokenizer::IsLetter(char c) const {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_')  || c == '\'' || (c >= 'а' && c <= 'я') || (c >= 'А' && c <= 'Я');
}

// проверка символа на цифру
bool Tokenizer::IsDigit(char c) const {
	return c >= '0' && c <= '9';
}

// проверка символа на букву или цифру
bool Tokenizer::IsLetterOrDigit(char c) const {
	return IsLetter(c) || IsDigit(c);
}

// перевод символа в нижний регистр
char Tokenizer::ToLower(char c) const {
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 'a';

	if (c >= 'А' && c <= 'Я')
		return c - 'А' + 'а';

	return c;
}

// удаление стоп слов
vector<string> Tokenizer::RemoveStopWords(vector<string> &tokens) {
	vector<string> cleared;

	for (int i = 0; i < tokens.size(); i++)
		if (stopWords.find(tokens[i]) == stopWords.end())
			cleared.push_back(tokens[i]);

	return cleared;
}

Tokenizer::Tokenizer() {
	ifstream f("stop.txt");
	string word;

	while (getline(f, word))
		stopWords.insert(word);

	f.close();
}

vector<string> Tokenizer::Tokenize(istream& f, bool removeStopWords) {
	vector<string> tokens;
	char c = f.get();

	while (!f.eof()) {
		while (!f.eof() && !IsLetterOrDigit(c))
			c = f.get();

		string token = "";

		while (!f.eof() && IsLetterOrDigit(c)) {
			token += ToLower(c);
			c = f.get();
		}

		if (token != "")
			tokens.push_back(token);
	}

	if (removeStopWords)
		return RemoveStopWords(tokens);

	return tokens;
}

vector<string> Tokenizer::Tokenize(const string& s, bool removeStopWords) {
	vector<string> tokens;
	int i = 0;

	while (i < s.length()) {
		while (i < s.length() && !IsLetterOrDigit(s[i]))
			i++;

		string token = "";

		while (i < s.length() && IsLetterOrDigit(s[i]))
			token += ToLower(s[i++]);

		if (token != "")
			tokens.push_back(token);
	}

	if (removeStopWords)
		return RemoveStopWords(tokens);

	return tokens;
}