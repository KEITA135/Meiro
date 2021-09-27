
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
# include <fstream>
# include <filesystem>
using namespace std;

void Main()
{

	//マップ読み込み
	ifstream in("input.txt");
	cin.rdbuf(in.rdbuf());
	int N, M;
	cin >> N >> M;
	vector<vector<char>> chizu(N, vector<char>(M));
	const Texture Wall(Resource(U"Wall.jpg"));
	const Texture Road(Resource(U"Road.jpg"));
	const Texture Back(Resource(U"Back.jpg"));
	const Texture Key(Emoji(U"🔑"));
	
	for (int i = 0;i < N;i++) {
		for (int j = 0;j < M;j++) cin >> chizu[i][j];
	}
	
	Scene::SetBackground(Palette::Cyan);
	Window::SetTitle(U"Meiro");
	const Font fontHeavy(30,Typeface::Heavy);

	//鍵の基本処理
	set<char> kagi;
	for (char i = 'a';i <= 'z';i++) kagi.insert(i);
	kagi.erase('s');
	kagi.erase('g');
	map<char, bool> opened;

	bool firstframe = true;
	pair<int, int> me = { 0,0 };
	vector<pair<int, int>> walked(0);
	while (System::Update()) {

		//マップの描画
		Back.resized(800, 600).draw(0, 0);
		for (int i = 0;i < N;i++) {
			for (int j = 0;j < M;j++) {
				Rect(600.0 / M * j + 100, 600.0 / N * i, 600.0 / M, 600.0 / N).draw(Palette::Yellow).drawFrame(1.0, Palette::Black);
				Road.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
				if (chizu[i][j] == '#') {
					Line(600.0 / M * j + 100, 600.0 / N * i, 600.0 / M * (j + 1) + 100, 600.0 / N * (i + 1)).draw(1.0, Palette::Black);
					Line(600.0 / M * (j + 1) + 100, 600.0 / N * i, 600.0 / M * j + 100, 600.0 / N * (i + 1)).draw(1.0, Palette::Black);
					Wall.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
				}
				if (chizu[i][j] == 's') {
					Circle(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2, min(600.0 / N, 600.0 / M) / 2 - 2).draw(Palette::Blue);
					if (firstframe) {
						me = { i,j };
						walked.push_back(me);
					}
				}
				if (chizu[i][j] == 'g') {
					Circle(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2, min(600.0 / N, 600.0 / M) / 2 - 2).draw(Palette::Red);
				}

				//鍵の描画
				if (kagi.count(chizu[i][j])) {
					if (firstframe) opened[chizu[i][j]] = false;
					if (!opened[chizu[i][j]]) {
						Key.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
						fontHeavy(U"{}"_fmt(chizu[i][j]-'a'+1)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100+2, 600.0 / N * (2 * i + 1) / 2+2,Palette::Gray);
						fontHeavy(U"{}"_fmt(chizu[i][j] - 'a' + 1)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2);
					}
				}
			}
		}
		for (int i = 1;i < walked.size();i++) {
			Line(600.0 / M * (2 * walked[i - 1].second + 1) / 2 + 100, 600.0 / N * (2 * walked[i - 1].first + 1) / 2, 600.0 / M * (2 * walked[i].second + 1) / 2 + 100, 600.0 / N * (2 * walked[i].first + 1) / 2).draw(5.0, Palette::Purple);
		}
		Circle(600.0 / M * (2 * me.second + 1) / 2 + 100, 600.0 / N * (2 * me.first + 1) / 2, min(600.0 / N, 600.0 / M) / 3).draw(Palette::Lightgreen).drawFrame(1.0,Palette::Black);

		//移動処理
		if (KeyUp.down()||KeyW.down()) {
			pair<int, int> after = { me.first-1,me.second };
			if (after.first != -1 && chizu[after.first][after.second] != '#') {
				if (walked.size() >= 2 && walked[walked.size() - 2] == after) walked.pop_back();
				else walked.push_back(after);
				me = after;
			}
		}
		if (KeyDown.down()||KeyS.down()) {
			pair<int, int> after = { me.first + 1,me.second };
			if (after.first != N && chizu[after.first][after.second] != '#') {
				if (walked.size() >= 2 && walked[walked.size() - 2] == after) walked.pop_back();
				else walked.push_back(after);
				me = after;
			}
		}
		if (KeyLeft.down()||KeyA.down()) {
			pair<int, int> after = { me.first ,me.second-1 };
			if (after.second != -1 && chizu[after.first][after.second] != '#') {
				if (walked.size() >= 2 && walked[walked.size() - 2] == after) walked.pop_back();
				else walked.push_back(after);
				me = after;
			}
		}
		if (KeyRight.down()||KeyD.down()) {
			pair<int, int> after = { me.first ,me.second+1 };
			if (after.second != M && chizu[after.first][after.second] != '#') {
				if (walked.size() >= 2 && walked[walked.size() - 2] == after) walked.pop_back();
				else walked.push_back(after);
				me = after;
			}
		}

		firstframe = false;
	}
}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D ユーザコミュニティ Slack への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
