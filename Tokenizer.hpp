#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Tokenizer {
	bool IsLetter(char c) const; // �������� ������� �� �����
	bool IsDigit(char c) const; // �������� ������� �� �����
	bool IsLetterOrDigit(char c) const; // �������� ������� �� ����� ��� �����
	char ToLower(char c) const; // ������� ������� � ������ �������
public:
	vector<string> Tokenize(istream& f);
	vector<string> Tokenize(const string& s);
};

// �������� ������� �� �����
bool Tokenizer::IsLetter(char c) const {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c >= '�' && c <= '�') || (c >= '�' && c <= '�');
}

// �������� ������� �� �����
bool Tokenizer::IsDigit(char c) const {
	return c >= '0' && c <= '9';
}

// �������� ������� �� ����� ��� �����
bool Tokenizer::IsLetterOrDigit(char c) const {
	return IsLetter(c) || IsDigit(c);
}

// ������� ������� � ������ �������
char Tokenizer::ToLower(char c) const {
	if (c >= 'A' && c <= 'Z')
		return c - 'A' + 'a';

	return c;
}

vector<string> Tokenizer::Tokenize(istream& f) {
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

	return tokens;
}

vector<string> Tokenizer::Tokenize(const string& s) {
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

	return tokens;
}