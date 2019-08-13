
#include <algorithm>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast/lexical_cast_old.hpp>

// base on sunday algorithm
std::size_t Search(std::string pattern,
	std::uintptr_t memory,
	std::size_t size,
	std::vector<std::size_t>& out) const
{
	if (pattern.length() == 0)
		return 0;
	std::vector<std::string> pattern_s;
	std::vector<std::uint8_t> pattern_b;
	boost::split(pattern_s, pattern, boost::is_any_of(" "));
	for (auto code : pattern_s) {
		if (code.length() != 2) {
			return 0;
		}
		if (code == "??")
			pattern_b.push_back('?');
		else
			pattern_b.push_back(static_cast<std::uint8_t>(toHex(code)));
	}

	std::size_t wildAddr = 0;
	for (std::size_t i = 0; i < pattern_b.size(); i++)
		if (pattern_b[i] == '?')
			wildAddr = i;
	std::size_t shift[257];
	for (auto i = 0; i < 256; i++)
		shift[i] = -1;
	for (std::size_t i = wildAddr + 1; i < pattern_b.size(); i++) {
		shift[pattern_b[i]] = pattern_b.size() - i;
	}
	for (int i = 0; i < 256; i++) {
		if (shift[i] == -1)
			shift[i] = pattern_b.size() - wildAddr;
	}

	std::size_t len_memory = size;
	std::size_t len_pattern = pattern_b.size();
	// search
	std::size_t i = 0;
	std::size_t j = 0;
	std::uint8_t* data = reinterpret_cast<std::uint8_t*>(memory);
	while (i <= len_memory - len_pattern) {
		while (j < len_pattern && (data[i + j] == pattern_b[j] || pattern_b[j] == '?'))
			j++;
		if (j == len_pattern) {
			out.push_back(i);
			i += len_pattern;
		}
		else {
			if (i + len_pattern >= len_memory)
				break;
			i += shift[data[i + len_pattern]];
		}
		j = 0;
	}
	return out.size();
}