# ifndef _block
# define _block
using namespace std;

struct block {
	string id;
	String name;
	int through;
	int life;

	block() {}

	block(string id) {
		this->id = id;
	}

	block(string id, String name) {
		this->id = id;
		this->name = name;
	}

	block(string id, String name, int through) {
		this->id = id;
		this->name = name;
		this->through = through;
	}

	block(string id, String name, int through,int life) {
		this->id = id;
		this->name = name;
		this->through = through;
		this->life = life;
	}
};

bool operator==(const block& a, const block& b);
bool operator<(const block& a, const block& b);

#endif