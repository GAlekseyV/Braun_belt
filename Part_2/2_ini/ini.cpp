#include "ini.h"
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <istream>
#include <string>
#include <string_view>
#include <utility>

using namespace std;

string LoadString(istream &input, char c = '\n')
{
  string line;
  getline(input, line, c);
  return move(line);
}

pair<string, string> Split(string line, char by)
{
  size_t pos = line.find(by);
  string left = line.substr(0, pos);

  if (pos < line.size() && pos + 1 < line.size()) {
    return { left, line.substr(pos + 1) };
  } else {
    return { left, string() };
  }
}

pair<string, string> LoadSection(istream &input)
{
  string line = LoadString(input);
  auto res = Split(line, '=');
  return res;
}

namespace Ini {

Document Load(std::istream &input)
{
  char c;
  input >> c;
  Document doc;

  while (!input.eof()) {
    if (c == '[') {
      Ini::Section *sec = &doc.AddSection(LoadString(input, ']'));
      LoadString(input);
      while (isalpha(input.peek())) {

        sec->insert(LoadSection(input));
      }
    }
    input >> c;
  }
  return doc;
}

Section &Document::AddSection(std::string name)
{
  return sections[name];
}

const Section &Document::GetSection(const std::string &name) const
{
  return sections.at(name);
}

size_t Document::SectionCount() const
{
  return sections.size();
}

}// namespace Ini
