# include <Siv3D.hpp>
# include "block.h"
using namespace std;

bool operator==(const block& a, const block& b) {
	if (a.id == b.id && a.name == b.name && a.through == b.through && a.life == b.life) return true;
	else return false;
}

bool operator<(const block& a, const block& b) {
	if (a.id < b.id) {
		return true;
	}
	return false;
}
