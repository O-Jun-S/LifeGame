#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define FIELD_WIDTH 94
#define FIELD_HEIGHT 46

// memo メモリ使用量1012KB(3/10)
enum alive {
	CELL_DEAD = 0,
	CELL_ALIVE = 1,
	CELL_MAX
};

// 8方向のベクトルの管理
enum vector {
	VECTOR_UP,
	VECTOR_DOWN,
	VECTOR_LEFT,
	VECTOR_RIGHT,
	VECTOR_UP_LEFT,
	VECTOR_UP_RIGHT,
	VECTOR_DOWN_LEFT,
	VECTOR_DOWN_RIGHT,
	VECTOR_MAX
};

int vectors[][2] = {
	{0, -1},  // VECTOR_UP
	{0, 1},  // VECTOR_DOWN
    {-1, 0},  // VECTOR_LEFT
	{1, 0},  // VECTOR_RIGHT
	{-1, -1},  // VECTOR_UP_LEFT
	{1, -1},  // VECTOR_UP_RIGHT
	{-1, 1},  // VECTOR_DOWN_LEFT
	{1, 1}  // VECTOR_DOWN_RIGHT
};

// 一つ一つの細胞の定義
int cells[FIELD_HEIGHT][FIELD_WIDTH];

// 計算するための値
int _cells[FIELD_HEIGHT][FIELD_WIDTH];

// カーソルの定義
int cursorX = 50, cursorY = 23;

// 最初に盤面のセットアップ
void setup(void) {
	for (int y = 0; y < FIELD_HEIGHT; y++)
		for (int x = 0; x < FIELD_WIDTH; x++) {
			cells[y][x] = CELL_DEAD;
		}
}

// 盤面の更新(firstはカーソルの表示非表示)
void update(bool cursor) {
	// 盤面のクリア
	system("cls");

	if (cursor) {
		printf("wasdで移動, rで命の配置, tで除去\n");
		printf("eでスタート\n");
	}

	else {
		printf("rで一世代進める\n");
	}
	for (int y = 0; y < FIELD_HEIGHT; y++) {
		for (int x = 0; x < FIELD_WIDTH; x++) {
			// カーソルの表示
			if (cursor) {
				if (x == cursorX && y == cursorY) {
					printf("◆");
				}

				else {
					if (cells[y][x] == CELL_DEAD) {
						printf("　");
					}

					if (cells[y][x] == CELL_ALIVE) {
						printf("●");
					}
                }
			}

			// 死んでいると□, 生きていると●
			else {
				if (cells[y][x] == CELL_DEAD) {
				    printf("　");
			    }

				if (cells[y][x] == CELL_ALIVE) {
					printf("●");
				}
			}
		}

		// FIELD_WIDTHごとに改行
		printf("\n");
	}
}

// 周りの生きているセルの数のカウント
int livesAlong(int _x, int _y) {
	// カウント
	int count = 0;

	for (int i = 0; i < VECTOR_MAX; i++) {
		int x2 = _x, y2 = _y;
		x2 += vectors[i][0];
		y2 += vectors[i][1];

		// はみ出さないように
		if (x2 < 0 || x2 >= FIELD_WIDTH) {
			continue;
		}

		if (y2 < 0 || y2 >= FIELD_HEIGHT) {
			continue;
		}

		// カウントの追加
		if (_cells[y2][x2] == CELL_ALIVE) {
			count += 1;
		}
	}

	return count;
}

// 次に生きているか(_alivesは周りの生きているセルの数, _cellは今のセルが生きているか死んでいるか)
bool nextAlive(int _alives, int _cell) {
	if (_cell == CELL_ALIVE) {
		switch (_alives) {
		case 1: return false; break;
		case 2: return true; break;
		case 3: return true; break;
		case 4: return false; break;
		}
	}

	if (_cell == CELL_DEAD) {
		if (_alives == 3) {
			return true;
		}

		else {
			return false;
		}
	}

	return false;
}

int main(void) {
	// 何世代目か
	int gen = 0;

	// 盤面のセットアップ
	setup();

	// ループ管理
	bool Set = true;

	int alvalg = 0;
	
    // 最初に命をセット
	while (Set) {
		update(true);
		switch (_getch()) {
	    // 移動
		case 'w': cursorY--; break;
		case 's': cursorY++; break;
		case 'a': cursorX--; break;
		case 'd': cursorX++; break;

		// 命を置く
		case 'r': cells[cursorY][cursorX] = CELL_ALIVE; break;
		case 't': cells[cursorY][cursorX] = CELL_DEAD; break;

		// ライフゲームを始める
		case 'e': Set = false; break;
		};

		// カーソルがはみ出さないように
		cursorX = (FIELD_WIDTH + cursorX) % FIELD_WIDTH;
		cursorY = (FIELD_HEIGHT + cursorY) % FIELD_HEIGHT;
	}

	// ライフゲームのループ(キー入力で次の世代, eで終わる)
	bool _continue = true;

	while (_continue) {
		update(false);
		printf("%d gen.", gen);
		switch (_getch()) {

		// rを押すと次の生死を判断(次の世代へ)
		case 'r':
			gen++;
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				for (int x = 0; x < FIELD_WIDTH; x++) {
					_cells[y][x] = cells[y][x];
				}
			}
			for (int y = 0; y < FIELD_HEIGHT; y++) {
				for (int x = 0; x < FIELD_WIDTH; x++) {
					if (nextAlive(livesAlong(x, y), _cells[y][x])) {
						cells[y][x] = CELL_ALIVE;
					}

					else {
						cells[y][x] = CELL_DEAD;
					}
				}
			}
			break;

		// eでプログラムを終える
		case 'e': _continue = false; break;
		default: break;
		}
	}
	return 0;
}