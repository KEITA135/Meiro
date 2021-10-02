
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
# include <vector>
# include <set>
# include <fstream>
# include <filesystem>
# include "App/block.h"
using namespace std;

void moveme(vector<vector<block>>& chizu, set<block>& haveItem, vector<pair<int, int>>& walked, pair<int, int>& me, pair<int, int>& after, int& HP);

void Main()
{

	//マップ読み込み
	ifstream in("input.txt");
	cin.rdbuf(in.rdbuf());
	int N, M;
	cin >> N >> M;
	int HP,HP_copy;
	cin >> HP;
	HP_copy = HP;
	vector<vector<block>> chizu(N, vector<block>(M));
	vector<vector<block>> chizu_copy(N, vector<block>(M));
	const Texture Wall(Resource(U"Wall.jpg"));
	const Texture Road(Resource(U"Road.jpg"));
	const Texture Road1(Resource(U"Road1.png"));
	const Texture Road2(Resource(U"Road2.png"));
	const Texture Road3(Resource(U"Road3.png"));
	const Texture Road4(Resource(U"Road4.png"));
	const Texture Back(Resource(U"Back.jpg"));
	const Texture Key(Emoji(U"🔑"));
	const Texture Door(Emoji(U"🚪"));
	const Texture Heart(Emoji(U"❤️"));
	
	int question = 0;
	for (int i = 0;i < N;i++) {
		for (int j = 0;j < M;j++) {
			char a;
			cin >> a;
			if (a == '.') chizu[i][j] = block(".");
			else if (a == '#') chizu[i][j] = block("#");
			else if (a == 's') chizu[i][j] = block("s");
			else if (a == 'g') chizu[i][j] = block("g");
			else if (a == '?') {
				question++;
			}
		}
	}
	
	for (int i = 0;i < question;i++) {
		int a, b;
		cin >> a >> b;
		string id;
		cin >> id;
		if (id == "Key") {
			String name;
			cin >> name;
			chizu[a][b] = block("Key", name,0);
		}
		if (id == "Door") {
			String name;
			cin >> name;
			chizu[a][b] = block("Door", name,0);
		}
		if (id == "Break") {
			int life;
			cin >> life;
			chizu[a][b] = block("Break", U"", 0,life);
		}
	}
	
	Scene::SetBackground(Palette::Cyan);
	Window::SetTitle(U"Meiro");
	const Font fontHeavy(30,Typeface::Heavy);
	const Font HPfont(20, Typeface::Heavy);
	const Font GameOver(140, Resource(U"851CHIKARA-YOWAKU_002.ttf"));
	const Font GameClear(115, Resource(U"NikkyouSans-mLKax.ttf"));

	bool firstframe = true;
	pair<int, int> me = { 0,0 };
	pair<int, int> me_copy = { 0,0 };
	vector<pair<int, int>> walked(0);
	set<block> haveItem;
	while (System::Update()) {
		//ステータスの描画
		ClearPrint();
		Print(U"Item:");
		for (block i : haveItem) {
			String id = U"?";
			if (i.id == "Key") id = U"Key";
			Print(U" {} ({})"_fmt(id, i.name));
		}
		if (!haveItem.size()) Print(U" None");

		//マップの描画
		Back.resized(800, 600).draw(0, 0);
		for (int i = 0;i < N;i++) {
			for (int j = 0;j < M;j++) {
				Rect(600.0 / M * j + 100, 600.0 / N * i, 600.0 / M, 600.0 / N).draw(Palette::Yellow).drawFrame(1.0, Palette::Black);
				Road.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
				if (chizu[i][j].id == "#") {
					Line(600.0 / M * j + 100, 600.0 / N * i, 600.0 / M * (j + 1) + 100, 600.0 / N * (i + 1)).draw(1.0, Palette::Black);
					Line(600.0 / M * (j + 1) + 100, 600.0 / N * i, 600.0 / M * j + 100, 600.0 / N * (i + 1)).draw(1.0, Palette::Black);
					Wall.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
				}
				if (chizu[i][j].id == "s") {
					Circle(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2, min(600.0 / N, 600.0 / M) / 2 - 2).draw(Palette::Blue);
					if (firstframe) {
						me = { i,j };
						walked.push_back(me);
					}
				}
				if (chizu[i][j].id == "g") {
					Circle(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2, min(600.0 / N, 600.0 / M) / 2 - 2).draw(Palette::Red);
				}

				//鍵の描画
				if (chizu[i][j].id == "Key") {
					if (chizu[i][j].through == 0) {
						Key.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
						fontHeavy(U"{}"_fmt(chizu[i][j].name)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100+2, 600.0 / N * (2 * i + 1) / 2+2,Palette::Gray);
						fontHeavy(U"{}"_fmt(chizu[i][j].name)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2);
					}
				}

				//扉の描画
				if (chizu[i][j].id == "Door") {
					if (chizu[i][j].through==0) {
						Door.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
						fontHeavy(U"{}"_fmt(chizu[i][j].name)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100 + 2, 600.0 / N * (2 * i + 1) / 2 + 2, Palette::Gray);
						fontHeavy(U"{}"_fmt(chizu[i][j].name)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2);
					}
				}

				//崩れる足場の描画
				if (chizu[i][j].id == "Break") {
					//通行可能
					if (chizu[i][j].through < chizu[i][j].life/3) {
						Road1.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
					}
					else if (chizu[i][j].through < chizu[i][j].life/2) {
						Road2.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
					}
					else if (chizu[i][j].through < chizu[i][j].life) {
						Road3.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
					}
					//通行不可
					else {
						Road4.resized(600.0 / M, 600.0 / N).draw(600.0 / M * j + 100, 600.0 / N * i);
					}

					fontHeavy(U"{}"_fmt(chizu[i][j].life - chizu[i][j].through)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100 + 2, 600.0 / N * (2 * i + 1) / 2 + 2, Palette::Gray);
					fontHeavy(U"{}"_fmt(chizu[i][j].life - chizu[i][j].through)).drawAt(600.0 / M * (2 * j + 1) / 2 + 100, 600.0 / N * (2 * i + 1) / 2);
				}
			}
		}
		for (int i = 1;i < walked.size();i++) {
			Line(600.0 / M * (2 * walked[i - 1].second + 1) / 2 + 100, 600.0 / N * (2 * walked[i - 1].first + 1) / 2, 600.0 / M * (2 * walked[i].second + 1) / 2 + 100, 600.0 / N * (2 * walked[i].first + 1) / 2).draw(5.0, Palette::Purple);
		}
		Circle(600.0 / M * (2 * me.second + 1) / 2 + 100, 600.0 / N * (2 * me.first + 1) / 2, min(600.0 / N, 600.0 / M) / 3).drawArc(0_deg,360_deg, min(600.0 / N, 600.0 / M) / 9,0,Palette::Lightgreen).drawFrame(1.0,Palette::Black);
		Circle(600.0 / M * (2 * me.second + 1) / 2 + 100, 600.0 / N * (2 * me.first + 1) / 2, min(600.0 / N, 600.0 / M) / 9 * 2 - 1).drawFrame(1.0, Palette::Black);
		Line(780, 560, 780, 40).draw(LineStyle::RoundCap, 15, Palette::White);
		if (HP_copy > 0) {
			Line(780, 560, 780, -520.0 * HP / HP_copy + 560).draw(LineStyle::RoundCap, 15, Palette::Red);
		}
		else Line(780, 560, 780, 40).draw(LineStyle::RoundCap, 15, Palette::Red);
		Heart.resized(20, 20).drawAt(780, 580);
		if (HP_copy > 0) {
			HPfont(U"{}"_fmt(HP)).drawAt(782, 22, Palette::Gray);
			HPfont(U"{}"_fmt(HP)).drawAt(780, 20, Palette::Red);
		}
		else {
			HPfont(U"inf").drawAt(782, 22, Palette::Gray);
			HPfont(U"inf").drawAt(780, 20, Palette::Red);
		}

		//ゲームオーバー画面
		if (!HP&&chizu[me.first][me.second].id!="g") {
			Rect(0, 0, 800, 600).draw(Color(0,0,0,120));
			GameOver(U"Game Over!").drawAt(400, 300, Palette::Red);
		}
		
		//ゲームクリア画面
		if (chizu[me.first][me.second].id == "g") {
			Rect(0, 0, 800, 600).draw(Color(255, 255, 255, 120));
			GameClear(U"Game Clear!").drawAt(400, 300, Palette::Gold);
		}

		//Copyの作成
		if (firstframe) {
			chizu_copy = chizu;
			me_copy = me;
		}

		//移動処理
		if (KeyUp.down()||KeyW.down()) {
			pair<int, int> after = { me.first-1,me.second };
			if (after.first != -1 && chizu[after.first][after.second].id != "#") {
				moveme(chizu, haveItem, walked, me, after, HP);
			}
		}
		if (KeyDown.down()||KeyS.down()) {
			pair<int, int> after = { me.first + 1,me.second };
			if (after.first != N && chizu[after.first][after.second].id != "#") {
				moveme(chizu, haveItem, walked, me, after, HP);
			}
		}
		if (KeyLeft.down()||KeyA.down()) {
			pair<int, int> after = { me.first ,me.second-1 };
			if (after.second != -1 && chizu[after.first][after.second].id != "#") {
				moveme(chizu, haveItem, walked, me, after, HP);
			}
		}
		if (KeyRight.down()||KeyD.down()) {
			pair<int, int> after = { me.first ,me.second+1 };
			if (after.second != M && chizu[after.first][after.second].id != "#") {
				moveme(chizu, haveItem, walked, me, after, HP);
			}
		}


		//ゲームリセット
		if (KeyR.down()) {
			chizu = chizu_copy;
			me = me_copy;
			HP = HP_copy;
			walked.clear();
			haveItem.clear();
			walked.push_back(me);
		}

		firstframe = false;
	}
}

//可能ならmeをafterに移動させる関数
void moveme(vector<vector<block>>& chizu,set<block>& haveItem,vector<pair<int,int>>& walked,pair<int,int>& me,pair<int,int>& after,int& HP) {
	bool move = true;

	if (!HP) move = false;
	if (chizu[me.first][me.second].id == "g") move = false;

	if (chizu[after.first][after.second].id == "Key" && chizu[after.first][after.second].through == 0) {
		haveItem.insert(chizu[after.first][after.second]);
		chizu[after.first][after.second].through = 1;
	}

	if (chizu[after.first][after.second].id == "Door" && chizu[after.first][after.second].through == 0) {
		if (haveItem.count(block("Key", chizu[after.first][after.second].name, 0))) {
			haveItem.erase(block("Key", chizu[after.first][after.second].name, 0));
			chizu[after.first][after.second].through = 1;
		}
		else {
			move = false;
		}
	}

	if (chizu[after.first][after.second].id == "Break") {
		if (!(chizu[after.first][after.second].life - chizu[after.first][after.second].through)) {
			move = false;
		}
	}

	if (move) {
		if (chizu[me.first][me.second].id == "Break") chizu[me.first][me.second].through++;
		if (walked.size() >= 2 && walked[walked.size() - 2] == after) walked.pop_back();
		else walked.push_back(after);
		me = after;
		HP--;
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
