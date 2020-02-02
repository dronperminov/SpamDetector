#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Tokenizer.hpp"

using namespace std;

// структруа для текста
struct Text {
	string label; // метка текста
	vector<string> tokens; // токены
};

class TextReader {
public:
	vector<Text> ReadTexts(const string& path); // считывание текстов
};

// считывание текстов
vector<Text> TextReader::ReadTexts(const string& path) {
	ifstream f(path); // открываем файл на чтение

	if (!f)
		throw string("Unable to open file '") + path + "'";

	vector<Text> texts; // вектор текстов
	string label, text; // метка и текст
	Tokenizer tokenizer;

	// пока считывается метка и текст
	while (f >> label && getline(f, text)) {
		stringstream ss(text); // создаём поток
		texts.push_back({ label, tokenizer.Tokenize(ss) }); // добавляем текст
	}

	f.close(); // закрываем файл

	return texts; // возвращаем тексты
}