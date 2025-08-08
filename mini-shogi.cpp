#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int BOARD_SIZE = 5;

enum Player { NONE = 0, PLAYER1, PLAYER2 };

struct Piece {
    char type; // 'P' for pawn, 'G' for gold, 'K' for king
    Player owner;
};

typedef vector<vector<Piece>> Board;

// 駒の表示
char displayPiece(Piece p) {
    if (p.owner == NONE) return '.';
    char symbol = p.type;
    if (p.owner == PLAYER2) symbol = tolower(symbol); // 後手は小文字
    return symbol;
}

// 初期化
Board initializeBoard() {
    Board board(BOARD_SIZE, vector<Piece>(BOARD_SIZE, {'.', NONE}));

    board[0][1] = {'G', PLAYER2};
    board[0][2] = {'K', PLAYER2};
    board[0][3] = {'G', PLAYER2};
    board[1][2] = {'P', PLAYER2};

    board[4][1] = {'G', PLAYER1};
    board[4][2] = {'K', PLAYER1};
    board[4][3] = {'G', PLAYER1};
    board[3][2] = {'P', PLAYER1};

    return board;
}

// 盤面表示
void printBoard(const Board& board) {
    cout << "  0 1 2 3 4" << endl;
    for (int y = 0; y < BOARD_SIZE; ++y) {
        cout << y << " ";
        for (int x = 0; x < BOARD_SIZE; ++x) {
            cout << displayPiece(board[y][x]) << " ";
        }
        cout << endl;
    }
}

// 盤外チェック
bool isInsideBoard(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

// 駒の合法手チェック（先手／後手に対応）
bool isLegalMove(Piece piece, int sx, int sy, int dx, int dy) {
    int dxRel = dx - sx;
    int dyRel = dy - sy;

    switch (piece.type) {
        case 'P':
            return dxRel == 0 && ((piece.owner == PLAYER1 && dyRel == -1) || (piece.owner == PLAYER2 && dyRel == 1));
        case 'G':
            if (piece.owner == PLAYER1) {
                return (abs(dxRel) <= 1 && dyRel >= -1 && dyRel <= 1) && !(dyRel == -1 && abs(dxRel) == 1);
            } else {
                return (abs(dxRel) <= 1 && dyRel >= -1 && dyRel <= 1) && !(dyRel == 1 && abs(dxRel) == 1);
            }
        case 'K':
            return abs(dxRel) <= 1 && abs(dyRel) <= 1;
    }
    return false;
}

// メイン処理
int main() {
    Board board = initializeBoard();
    Player turn = PLAYER1;

    while (true) {
        printBoard(board);
        cout << "Player " << ((turn == PLAYER1) ? "1" : "2") << "'s turn." << endl;

        int sx, sy, dx, dy;
        cout << "Enter move (sy sx dy dx): ";
        cin >> sy >> sx >> dy >> dx;

        if (!isInsideBoard(sx, sy) || !isInsideBoard(dx, dy)) {
            cout << "Invalid coordinates. Try again.\n";
            continue;
        }

        Piece& from = board[sy][sx];
        Piece& to = board[dy][dx];

        if (from.owner != turn) {
            cout << "Not your piece. Try again.\n";
            continue;
        }

        if (!isLegalMove(from, sx, sy, dx, dy)) {
            cout << "Illegal move. Try again.\n";
            continue;
        }

        if (to.owner == turn) {
            cout << "You can't capture your own piece.\n";
            continue;
        }

        if (to.type == 'K') {
            cout << "Player " << ((turn == PLAYER1) ? "1" : "2") << " wins!\n";
            break;
        }

        to = from;
        from = {'.', NONE};
        turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
    }

    return 0;
}

