//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <vector>
#include <cstdlib>
#include <pngimage.hpp>
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TForm1 *Form1;

TPiece Board[8][8];
PieceColor CurrentPlayer;
int SelectedX = -1, SelectedY = -1;
TStringList *GameHistory;
bool IsGameOver = false;
bool PossibleMoves[8][8];
int GameSeconds = 0;
bool GameTimerRunning = true;
bool CheckFlag = false;
bool IsCheckmate = false;
bool IsStalemate = false;
bool IsDrawByMaterial = false;
int MoveCount = 0;
int HalfMoveCounter = 0;
bool EnPassantPossible = false;
int EnPassantRow = -1;
int EnPassantCol = -1;
bool WhiteKingMoved = false;
bool BlackKingMoved = false;
bool WhiteLeftRookMoved = false;
bool WhiteRightRookMoved = false;
bool BlackLeftRookMoved = false;
bool BlackRightRookMoved = false;
int BotFromRow = -1;
int BotFromCol = -1;
int BotToRow = -1;
int BotToCol = -1;
AnsiString GameOverMessage = "";

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner)
{
	GameHistory = new TStringList();
	SetupBoard();
	DrawBoard();
	UpdateStatusLabel();
	UpdateGameTimerLabel();
	Label1->Caption = "Нажмите 'Новая игра'";

	Timer1->Interval = 1000;
	Timer1->Enabled = false;
	GameTimerRunning = false;

	GameMode = 0;

	WhiteKingImg = new TPngImage();
	WhiteQueenImg = new TPngImage();
	WhiteRookImg = new TPngImage();
	WhiteBishopImg = new TPngImage();
	WhiteKnightImg = new TPngImage();
	WhitePawnImg = new TPngImage();

	BlackKingImg = new TPngImage();
	BlackQueenImg = new TPngImage();
	BlackRookImg = new TPngImage();
	BlackBishopImg = new TPngImage();
	BlackKnightImg = new TPngImage();
	BlackPawnImg = new TPngImage();

    WhiteKingImg->LoadFromFile("images\\white king.png");
	WhiteQueenImg->LoadFromFile("images\\white queen.png");
	WhiteRookImg->LoadFromFile("images\\white rook.png");
	WhiteBishopImg->LoadFromFile("images\\white bishop.png");
	WhiteKnightImg->LoadFromFile("images\\white knight.png");
	WhitePawnImg->LoadFromFile("images\\white pawn.png");

	BlackKingImg->LoadFromFile("images\\black king.png");
	BlackQueenImg->LoadFromFile("images\\black queen.png");
	BlackRookImg->LoadFromFile("images\\black rook.png");
	BlackBishopImg->LoadFromFile("images\\black bishop.png");
	BlackKnightImg->LoadFromFile("images\\black knight.png");
	BlackPawnImg->LoadFromFile("images\\black pawn.png");

	ComboBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void TForm1::SetupBoard()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			Board[i][j] = TPiece(Empty, None);

	for (int j = 0; j < 8; j++) {
		Board[1][j] = TPiece(Black, Pawn);
		Board[6][j] = TPiece(White, Pawn);
	}

	Board[0][0] = TPiece(Black, Rook);
	Board[0][1] = TPiece(Black, Knight);
    Board[0][2] = TPiece(Black, Bishop);
	Board[0][3] = TPiece(Black, Queen);
	Board[0][4] = TPiece(Black, King);
	Board[0][5] = TPiece(Black, Bishop);
	Board[0][6] = TPiece(Black, Knight);
	Board[0][7] = TPiece(Black, Rook);

	Board[7][0] = TPiece(White, Rook);
    Board[7][1] = TPiece(White, Knight);
	Board[7][2] = TPiece(White, Bishop);
	Board[7][3] = TPiece(White, Queen);
	Board[7][4] = TPiece(White, King);
    Board[7][5] = TPiece(White, Bishop);
	Board[7][6] = TPiece(White, Knight);
	Board[7][7] = TPiece(White, Rook);
}
//---------------------------------------------------------------------------

void TForm1::DrawBoard()
{
	int cellSize = Image1->Width / 8;
	Graphics::TBitmap *buffer = new Graphics::TBitmap();
	buffer->Width = Image1->Width;
	buffer->Height = Image1->Height;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			int x = col * cellSize;
			int y = row * cellSize;

			if ((row + col) % 2 == 0)
				buffer->Canvas->Brush->Color = (TColor)RGB(240, 217, 181);
			else
				buffer->Canvas->Brush->Color = (TColor)RGB(181, 136, 99);

			if ((row == BotFromRow && col == BotFromCol) || (row == BotToRow &&
			 col == BotToCol))
			{
				buffer->Canvas->Brush->Color = clYellow;
			}

			if (SelectedX == col && SelectedY == row)
				buffer->Canvas->Brush->Color = clYellow;
			else if (PossibleMoves[row][col]) {
				if (Board[row][col].Type != None)
					buffer->Canvas->Brush->Color = clRed;
				else
					buffer->Canvas->Brush->Color = clLime;
			}

			buffer->Canvas->FillRect(Rect(x, y, x + cellSize, y + cellSize));

			if (Board[row][col].Type != None)
			{
				TPngImage *img = NULL;

				if (Board[row][col].Color == White)
				{
					switch(Board[row][col].Type)
					{
						case King:   img = WhiteKingImg; break;
						case Queen:  img = WhiteQueenImg; break;
						case Rook:   img = WhiteRookImg; break;
						case Bishop: img = WhiteBishopImg; break;
						case Knight: img = WhiteKnightImg; break;
						case Pawn:   img = WhitePawnImg; break;
					}
				}
				else
				{
					switch(Board[row][col].Type)
					{
						case King:   img = BlackKingImg; break;
						case Queen:  img = BlackQueenImg; break;
						case Rook:   img = BlackRookImg; break;
						case Bishop: img = BlackBishopImg; break;
						case Knight: img = BlackKnightImg; break;
						case Pawn:   img = BlackPawnImg; break;
					}
				}

				if (img)
				{
					buffer->Canvas->StretchDraw(
						Rect(
							x + 4,
							y + 4,
							x + cellSize - 4,
							y + cellSize - 4
						),
						img
					);
				}
			}
		}
	}

	Image1->Canvas->Draw(0, 0, buffer);
	delete buffer;
}
//---------------------------------------------------------------------------

void TForm1::ClearPossibleMoves()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			PossibleMoves[i][j] = false;
}
//---------------------------------------------------------------------------

void TForm1::CalculatePossibleMoves(int row, int col)
{
	ClearPossibleMoves();
	if (Board[row][col].Type == None) return;

	for (int tr = 0; tr < 8; tr++)
		for (int tc = 0; tc < 8; tc++)
			if (IsMoveValid(row, col, tr, tc))
				PossibleMoves[tr][tc] = true;
}
//---------------------------------------------------------------------------

void TForm1::ExecuteMove(int fromRow, int fromCol, int toRow, int toCol)
{
	TPiece movingPiece = Board[fromRow][fromCol];

	bool castling = movingPiece.Type == King && abs(toCol - fromCol) == 2;

	bool resetCounter = false;

	if (Board[toRow][toCol].Type != None)
		resetCounter = true;

	if (Board[fromRow][fromCol].Type == Pawn)
		resetCounter = true;

	if (movingPiece.Type == Pawn && EnPassantPossible && toRow == EnPassantRow &&
	toCol == EnPassantCol && Board[toRow][toCol].Type == None)
	{
		if (movingPiece.Color == White)
			Board[toRow + 1][toCol] = TPiece(Empty, None);
		else
			Board[toRow - 1][toCol] = TPiece(Empty, None);

		resetCounter = true;

	}

	Board[toRow][toCol] = Board[fromRow][fromCol];

	Board[fromRow][fromCol] = TPiece(Empty, None);

    // запоминаем факт хода короля
	if (movingPiece.Type == King)
	{
		if (movingPiece.Color == White)
			WhiteKingMoved = true;
		else
			BlackKingMoved = true;
	}

	// запоминаем факт хода ладьи
	if (movingPiece.Type == Rook)
	{
		if (fromRow == 7 && fromCol == 0)
			WhiteLeftRookMoved = true;

		if (fromRow == 7 && fromCol == 7)
			WhiteRightRookMoved = true;

		if (fromRow == 0 && fromCol == 0)
			BlackLeftRookMoved = true;

		if (fromRow == 0 && fromCol == 7)
			BlackRightRookMoved = true;
	}

	if (castling)
	{
		// короткая
		if (toCol == 6)
		{
			Board[toRow][5] = Board[toRow][7];
			Board[toRow][7] = TPiece(Empty, None);
		}
		// длинная
		else if (toCol == 2)
		{
			Board[toRow][3] = Board[toRow][0];
			Board[toRow][0] = TPiece(Empty, None);
		}
	}

	if (Board[toRow][toCol].Type == Pawn) {
		if (Board[toRow][toCol].Color == White && toRow == 0)
			Board[toRow][toCol].Type = Queen;
		else if (Board[toRow][toCol].Color == Black && toRow == 7)
			Board[toRow][toCol].Type = Queen;
	}

	if (resetCounter)
		HalfMoveCounter = 0;
	else
		HalfMoveCounter++;

	EnPassantPossible = false;

	if (movingPiece.Type == Pawn && abs(toRow - fromRow) == 2)
	{
		EnPassantPossible = true;
		EnPassantRow = (fromRow + toRow) / 2;
		EnPassantCol = fromCol;
	}
}
//---------------------------------------------------------------------------

bool TForm1::IsMoveValid(int fromRow, int fromCol, int toRow, int toCol)
{
    TPiece piece = Board[fromRow][fromCol];
	TPiece target = Board[toRow][toCol];

	if (piece.Type == None) return false;
	if (target.Color == piece.Color) return false;

	bool moveValid = false;
	int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

	switch(piece.Type) {
		case Pawn: {
			int direction = (piece.Color == White) ? -1 : 1;
			int startRow = (piece.Color == White) ? 6 : 1;

			//ход на одну клетку вперед
			if (toCol == fromCol && toRow == fromRow + direction && target.Type == None)
				moveValid = true;
			//ход на 2 клетки из начальной позиции
			if (toCol == fromCol && toRow == fromRow + 2*direction &&
				fromRow == startRow && target.Type == None &&
				Board[fromRow + direction][fromCol].Type == None)
				moveValid = true;
			//обычное взятие
			if (abs(toCol - fromCol) == 1 && toRow == fromRow + direction &&
				target.Type != None && target.Color != piece.Color)
			{
				moveValid = true;
			}
            //взятие на проходе
			if (abs(toCol - fromCol) == 1 && toRow == fromRow + direction &&
				target.Type == None && EnPassantPossible && toRow == EnPassantRow &&
				toCol == EnPassantCol)
			{
				moveValid = true;
			}
			break;
		}
		case Knight:
			if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2))
				moveValid = true;
			break;
		case King:
			if (rowDiff <= 1 && colDiff <= 1)
			{
				moveValid = true;
			}
			else if (rowDiff == 0 && colDiff == 2)
			{
				// попытка рокировки

				if (piece.Color == White)
				{
					if (WhiteKingMoved)
						return false;

					// короткая рокировка
					if (toCol == 6)
					{
						if (WhiteRightRookMoved)
							return false;

						if (Board[7][7].Type != Rook ||
							Board[7][7].Color != White)
							return false;


						if (Board[7][5].Type != None ||
							Board[7][6].Type != None)
							return false;

						if (IsCheck(White))
							return false;

						if (IsSquareUnderAttack(7,5,Black) ||
							IsSquareUnderAttack(7,6,Black))
							return false;

						moveValid = true;
					}

					// длинная рокировка
					if (toCol == 2)
					{
						if (WhiteLeftRookMoved)
							return false;

                        if (Board[7][0].Type != Rook ||
							Board[7][0].Color != White)
							return false;

						if (Board[7][1].Type != None ||
							Board[7][2].Type != None ||
							Board[7][3].Type != None)
							return false;

						if (IsCheck(White))
							return false;

						if (IsSquareUnderAttack(7,3,Black) ||
							IsSquareUnderAttack(7,2,Black))
							return false;

						moveValid = true;
					}
				}
				else
				{
					if (BlackKingMoved)
						return false;

					// короткая
					if (toCol == 6)
					{
						if (BlackRightRookMoved)
							return false;

                        if (Board[0][7].Type != Rook ||
							Board[0][7].Color != Black)
							return false;

						if (Board[0][5].Type != None ||
							Board[0][6].Type != None)
							return false;

						if (IsCheck(Black))
							return false;

						if (IsSquareUnderAttack(0,5,White) ||
							IsSquareUnderAttack(0,6,White))
							return false;

						moveValid = true;
					}

					// длинная
					if (toCol == 2)
					{
						if (BlackLeftRookMoved)
							return false;

                        if (Board[0][0].Type != Rook ||
							Board[0][0].Color != Black)
							return false;

						if (Board[0][1].Type != None ||
							Board[0][2].Type != None ||
							Board[0][3].Type != None)
							return false;

						if (IsCheck(Black))
							return false;

						if (IsSquareUnderAttack(0,3,White) ||
							IsSquareUnderAttack(0,2,White))
							return false;

						moveValid = true;
					}
				}
			}
			break;
		case Queen:
			if (fromRow == toRow || fromCol == toCol || rowDiff == colDiff)
				moveValid = true;
			break;
		case Rook:
			if (fromRow == toRow || fromCol == toCol)
				moveValid = true;
			break;
		case Bishop:
			if (rowDiff == colDiff)
				moveValid = true;
            break;
		default:
			moveValid = false;
			break;
    }

    if (!moveValid) return false;

	if (piece.Type == Rook || piece.Type == Bishop || piece.Type == Queen) {
		int rowStep = 0;
        int colStep = 0;

		if (toRow > fromRow) rowStep = 1;
		else if (toRow < fromRow) rowStep = -1;

		if (toCol > fromCol) colStep = 1;
		else if (toCol < fromCol) colStep = -1;

		int r = fromRow + rowStep;
		int c = fromCol + colStep;

		while (r != toRow || c != toCol) {
			if (Board[r][c].Type != None) return false;
			r += rowStep;
			c += colStep;
		}
	}

	TPiece savedFrom = Board[fromRow][fromCol];
	TPiece savedTo = Board[toRow][toCol];

	Board[toRow][toCol] = Board[fromRow][fromCol];
	Board[fromRow][fromCol] = TPiece(Empty, None);

	bool kingInCheck = IsCheck(savedFrom.Color);

	Board[fromRow][fromCol] = savedFrom;
	Board[toRow][toCol] = savedTo;

	if (kingInCheck) return false;

	return true;
}

bool TForm1::IsSquareUnderAttack(
	int row,
	int col,
	PieceColor attacker
)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (Board[r][c].Color != attacker)
                continue;

            if (CanPieceAttackSquare(r, c, row, col))
                return true;
        }
    }

    return false;
}

bool TForm1::CanPieceAttackSquare(int fromRow, int fromCol, int toRow, int toCol)
{
	if (fromRow == toRow && fromCol == toCol)
	{
		return false;
	}

	TPiece piece = Board[fromRow][fromCol];

	if (piece.Type == None)
        return false;

    int rowDiff = abs(toRow - fromRow);
    int colDiff = abs(toCol - fromCol);

    bool canAttack = false;

    switch(piece.Type)
	{
		case Pawn:
        {
			int direction =
                (piece.Color == White) ? -1 : 1;

            if (toRow == fromRow + direction &&
				abs(toCol - fromCol) == 1)
            {
				canAttack = true;
            }
			break;
        }

        case Knight:
			canAttack =
                (rowDiff == 2 && colDiff == 1) ||
				(rowDiff == 1 && colDiff == 2);
            break;

        case King:
			canAttack =
                rowDiff <= 1 &&
				colDiff <= 1;
            break;

        case Bishop:
			canAttack =
                (rowDiff == colDiff);
			break;

		case Rook:
            canAttack =
				(fromRow == toRow ||
                 fromCol == toCol);
			break;

		case Queen:
            canAttack =
				(fromRow == toRow ||
				 fromCol == toCol ||
				 rowDiff == colDiff);
			break;
	}

	if (!canAttack)
		return false;

	if (piece.Type == Bishop ||
		piece.Type == Rook ||
		piece.Type == Queen)
	{
		int rowStep = 0;
		int colStep = 0;

		if (toRow > fromRow) rowStep = 1;
		else if (toRow < fromRow) rowStep = -1;

		if (toCol > fromCol) colStep = 1;
		else if (toCol < fromCol) colStep = -1;

		int r = fromRow + rowStep;
		int c = fromCol + colStep;

		while (r != toRow || c != toCol)
		{
			if (Board[r][c].Type != None)
				return false;

			r += rowStep;
			c += colStep;
		}
	}

	return true;
}

int TForm1::CountAttackers(int row, int col, PieceColor color)
{
	int count = 0;

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
        {
            if (Board[r][c].Color != color)
                continue;

            if (CanPieceAttackSquare(r,c,row,col))
                count++;
        }
    }

    return count;
}

int TForm1::AttackValue(int row, int col, PieceColor color)
{
    int value = 0;

    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (Board[r][c].Color != color)
                continue;

            if (CanPieceAttackSquare(r,c,row,col))
            {
                value +=
                    GetPieceValue(Board[r][c]);
            }
        }
    }

    return value;
}

bool TForm1::IsCheck(PieceColor kingColor)
{
	// Находим короля
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (Board[r][c].Type == King && Board[r][c].Color == kingColor) {
				// Проверяем, атакован ли король
				PieceColor attacker = (kingColor == White) ? Black : White;
				return IsSquareUnderAttack(r, c, attacker);
			}
		}
	}
	return false;
}
bool TForm1::HasLegalMoves(PieceColor player)
{
	// Проверяем все фигуры игрока
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (Board[r][c].Color == player && Board[r][c].Type != None) {
				// Пробуем все возможные клетки для хода
				for (int tr = 0; tr < 8; tr++) {
					for (int tc = 0; tc < 8; tc++) {
						if (IsMoveValid(r, c, tr, tc)) {
							return true;  // Нашли хотя бы один ход
						}
					}
				}
			}
		}
	}
	return false;  // Нет ни одного хода
}

void TForm1::UpdateStatusLabel()
{
  if (IsGameOver) {
    Label1->Caption = GameOverMessage;
    return;
  }

  if (!GameTimerRunning) {
    Label1->Caption = "Нажмите 'Новая игра'";
    return;
  }

  if (CheckFlag) {
    Label1->Caption = (CurrentPlayer == White) ? "Шах! Ход: Белые" : "Шах! Ход: Чёрные";
  }
  else {
    Label1->Caption = (CurrentPlayer == White) ? "Ход: Белые" : "Ход: Чёрные";
  }

}



//---------------------------------------------------------------------------

void TForm1::UpdateGameTimerLabel()
{
	int minutes = GameSeconds / 60;
	int seconds = GameSeconds % 60;
	Label2->Caption = AnsiString().sprintf("Время: %02d:%02d", minutes, seconds);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	if (GameTimerRunning && !IsGameOver) {
		GameSeconds++;
		UpdateGameTimerLabel();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button,
TShiftState Shift, int X, int Y)
{
	if (IsGameOver) return;

    // ===== ЕСЛИ ХОДИТ КОМПЬЮТЕР — ИГНОРИРУЕМ КЛИКИ =====
	if (GameMode != 0 && CurrentPlayer == Black) {
		return;
	}

    int cellSize = Image1->Width / 8;
    int col = X / cellSize;
    int row = Y / cellSize;

    if (row < 0 || row > 7 || col < 0 || col > 7) return;

    // =========================
    // 1. ВЫБОР ФИГУРЫ
    // =========================
    if (SelectedX == -1)
    {
        if (Board[row][col].Color == CurrentPlayer)
        {
            SelectedX = col;
            SelectedY = row;
            CalculatePossibleMoves(row, col);
        }

        DrawBoard();
        return;
    }

    // =========================
    // 2. ПОПЫТКА ХОДА
    // =========================
	if (PossibleMoves[row][col])
	{
		int fromRow = SelectedY;
		int fromCol = SelectedX;
		TPiece movingPiece = Board[fromRow][fromCol];
		bool capture = Board[row][col].Type != None;
		if (movingPiece.Type == Pawn &&
			EnPassantPossible &&
			row == EnPassantRow &&
			col == EnPassantCol &&
			Board[row][col].Type == None)
		{
			capture = true;
		}
		ExecuteMove(fromRow, fromCol, row, col);
		MoveCount++;
		SelectedX = -1;
		SelectedY = -1;
		ClearPossibleMoves();
		UpdateGameState();
		AddMoveToHistory(
			fromRow, fromCol,
			row, col,
			movingPiece,
			capture,
			CheckFlag
		);

		if (CheckGameOver())
		{
			DrawBoard();
			UpdateStatusLabel();
			return;
		}

		CurrentPlayer = (CurrentPlayer == White) ? Black : White;

        if (GameMode != 0 && CurrentPlayer == Black)
		{
			SmartMove();
		}
    }
	else
	{
        if (Board[row][col].Color == CurrentPlayer)
        {
            SelectedX = col;
            SelectedY = row;
			CalculatePossibleMoves(row, col);
        }
        else
		{
            SelectedX = -1;
            SelectedY = -1;
			ClearPossibleMoves();
        }
    }

    DrawBoard();
	UpdateStatusLabel();
}
//---------------------------------------------------------------------------
void TForm1::AddMoveToHistory(int fromRow, int fromCol, int toRow, int toCol, TPiece piece, bool capture,bool check)
{
    AnsiString player =
        (piece.Color == White) ? "Белые" : "Чёрные";
	AnsiString pieceName;
    switch (piece.Type)
    {
        case Pawn:   pieceName = "Пешка"; break;
        case Knight: pieceName = "Конь"; break;
        case Bishop: pieceName = "Слон"; break;
        case Rook:   pieceName = "Ладья"; break;
        case Queen:  pieceName = "Ферзь"; break;
        case King:   pieceName = "Король"; break;
    }
    char fromFile = 'A' + fromCol;
    int fromRank = 8 - fromRow;
    char toFile = 'A' + toCol;
    int toRank = 8 - toRow;
    GameHistory->Add(
        "Ход " + IntToStr(MoveCount)
    );
    GameHistory->Add(
        "Игрок: " + player
    );
    GameHistory->Add(
        "Фигура: " + pieceName
    );
    GameHistory->Add(
        "Ход: " +
        AnsiString(fromFile) + IntToStr(fromRank) +
        " -> " +
        AnsiString(toFile) + IntToStr(toRank)
    );
    GameHistory->Add(
        "Взятие: " + String(capture ? "Да" : "Нет")
    );
    GameHistory->Add(
        "Шах: " + String(check ? "Да" : "Нет")
    );
    GameHistory->Add("");
}

bool TForm1::CheckGameOver()
{
	 UpdateGameState();

	 if (IsCheckmate) {
		  IsGameOver = true;
		  GameTimerRunning = false;
		  Timer1->Enabled = false;

		  SelectedX = -1;
		  SelectedY = -1;
		  ClearPossibleMoves();
		  DrawBoard();

		  AnsiString winner = (CurrentPlayer == White) ? "Белые" : "Черные";
		  GameOverMessage = winner + " победили! Мат!";

		  return true;
	 }

	 if (IsStalemate) {
		  IsGameOver = true;
		  GameTimerRunning = false;
		  Timer1->Enabled = false;

		  SelectedX = -1;
		  SelectedY = -1;
		  ClearPossibleMoves();
		  DrawBoard();

		  GameOverMessage = "Пат! Ничья!";
		  return true;
	 }

	if (IsDrawByMaterial) {
		IsGameOver = true;
		GameOverMessage = "Ничья: недостаточно материала";
		return true;
	}

	if (HalfMoveCounter >= 100) {
		IsGameOver = true;
		GameOverMessage = "Ничья по правилу 50 ходов";
	}


	 return false;
}

bool TForm1::InsufficientMaterial()
{
    int bishops = 0;
    int knights = 0;
    int otherPieces = 0;

    for (int r = 0; r < 8; r++)
	{
        for (int c = 0; c < 8; c++)
        {
            TPiece p = Board[r][c];

            if (p.Type == None || p.Type == King)
                continue;

            switch (p.Type)
            {
                case Bishop: bishops++; break;
                case Knight: knights++; break;

                default:
                    otherPieces++;
            }
		}
    }

    if (otherPieces > 0)
        return false;

    int minorPieces = bishops + knights;

    return minorPieces <= 1;
}

void TForm1::UpdateGameState()
{
    PieceColor sideToMove =
        (CurrentPlayer == White)
        ? Black
        : White;

    CheckFlag = IsCheck(sideToMove);

    bool hasMoves =
        HasLegalMoves(sideToMove);

	IsCheckmate =
        CheckFlag && !hasMoves;

    IsStalemate =
        !CheckFlag && !hasMoves;

    IsDrawByMaterial =
		InsufficientMaterial();
}

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	SetupBoard();
    GameHistory->Clear();
	CheckFlag = false;
	IsCheckmate = false;
	IsStalemate = false;
	CurrentPlayer = White;
	SelectedX = -1;
	SelectedY = -1;
	IsGameOver = false;
	GameSeconds = 0;
	MoveCount = 0;
	WhiteKingMoved = false;
	BlackKingMoved = false;
	WhiteLeftRookMoved = false;
	WhiteRightRookMoved = false;
	BlackLeftRookMoved = false;
	BlackRightRookMoved = false;
	GameOverMessage = "";
    BotFromRow = -1;
	BotFromCol = -1;
	BotToRow = -1;
	BotToCol = -1;
	ClearPossibleMoves();
	DrawBoard();
	UpdateGameTimerLabel();

	GameTimerRunning = true;
	Timer1->Enabled = true;
	UpdateGameState();
	UpdateStatusLabel();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
    int newMode = ComboBox1->ItemIndex;

    // Если партия уже идёт
    if (GameTimerRunning && !IsGameOver)
    {
        int result = Application->MessageBox(
            L"Закончить текущую партию?",
            L"Смена режима",
            MB_YESNO | MB_ICONQUESTION
        );

        if (result == IDNO)
        {
            // Возвращаем старый режим в ComboBox
            ComboBox1->ItemIndex = GameMode;
            return;
        }

        // Игру останавливаем
        GameTimerRunning = false;
        Timer1->Enabled = false;

        Label1->Caption =
            "Режим изменён. Нажмите 'Новая игра'";
    }

    GameMode = newMode;
}

int TForm1::GetPieceValue(TPiece piece)
{
	if (piece.Type == None) return 0;

	switch(piece.Type) {
		case Pawn:   return 1;
		case Knight: return 3;
		case Bishop: return 3;
		case Rook:   return 5;
		case Queen:  return 9;
        case King:   return 100;
		default:     return 0;
    }
}

int TForm1::EvaluateMove(int fromRow, int fromCol, int toRow, int toCol)
{
	TPiece piece = Board[fromRow][fromCol];
	TPiece target = Board[toRow][toCol];

    int score = 0;

	// взятие фигуры
	if (target.Type != None) {
		score += GetPieceValue(target) * 10 - GetPieceValue(piece) * 5;
	}

	// шах (проверяем временным ходом)
	TPiece savedFrom = Board[fromRow][fromCol];
	TPiece savedTo = Board[toRow][toCol];

	Board[toRow][toCol] = Board[fromRow][fromCol];
	Board[fromRow][fromCol] = TPiece(Empty, None);

	PieceColor opponent = (piece.Color == White) ? Black : White;

	int attackers = CountAttackers(toRow,toCol,opponent);

	int defenders = CountAttackers(toRow,toCol,piece.Color);

    if (attackers > defenders)
	{
		score -=
			GetPieceValue(piece) * 20;
	}



    if (IsCheck(opponent) &&
    CountAttackers(toRow,toCol,piece.Color) >
    CountAttackers(toRow,toCol,opponent))
	{
		score += 10;
	}

	// проверяем мат
	if (IsCheck(opponent) && !HasLegalMoves(opponent)) {
		score += 999999;  // Максимальный бонус за мат
	}

	Board[fromRow][fromCol] = savedFrom;
	Board[toRow][toCol] = savedTo;

	// продвижение пешки
	if (piece.Type == Pawn) {
		int direction = (piece.Color == White) ? -1 : 1;

		int targetRow = (piece.Color == White) ? toRow : (7 - toRow);
		score += targetRow;
	}

	if (MoveCount < 12 && piece.Type == Pawn)
	{
		if (fromCol == 3 || fromCol == 4) // d,e
			score += 4;
		else
			score += 3;
	}

	// центр доски
    if (CanPieceAttackSquare(toRow,toCol,3,3))
    score += 5;

	if (CanPieceAttackSquare(toRow,toCol,3,4))
		score += 5;

	if (CanPieceAttackSquare(toRow,toCol,4,3))
		score += 5;

	if (CanPieceAttackSquare(toRow,toCol,4,4))
		score += 5;

	// открытые вертикали
	if (piece.Type == Rook || piece.Type == Queen)
	{
		if (IsOpenFile(toCol))
			score += 15;
	}

	bool ownPawn = false;
	// полуоткрытые вертикали
	for (int r = 0; r < 8; r++)
	{
		if (Board[r][toCol].Type == Pawn &&
			Board[r][toCol].Color == piece.Color)
		{
			ownPawn = true;
			break;
		}
	}

	if (!ownPawn)
		score += 8;
	// развитие коней и слонов, штраф за ранний выход ферзя и ладьи
	if (MoveCount < 12)
	{
		if (piece.Type == Knight || piece.Type == Bishop)
			score += 8;

		if (piece.Type == Queen)
			score -= 15;

		if (piece.Type == Rook)
			score -= 10;
	}

	//ходы королем
	if (MoveCount < 20 && piece.Type == King)
	{
		if (abs(toCol - fromCol) == 2)
			score += 40; //рокировка

		else if (!IsCheck)
			score -= 20; //бесполезный ход
	}

	return score;
}

int TForm1::OpponentBestResponse(PieceColor opponent)
{
	int best = -999999;

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (Board[r][c].Color != opponent)
				continue;

			for (int tr = 0; tr < 8; tr++)
			{
                for (int tc = 0; tc < 8; tc++)
                {
                    if (!IsMoveValid(r,c,tr,tc))
                        continue;

                    TPiece savedFrom = Board[r][c];
                    TPiece savedTo   = Board[tr][tc];

                    Board[tr][tc] = Board[r][c];
                    Board[r][c] = TPiece(Empty,None);

                    int score = EvaluateMove(r, c, tr, tc);

					if (IsCheck(Black))
						score += 50;

					if (IsCheck(Black) && !HasLegalMoves(Black))
					{
						score += 999999;
					}

                    if (score > best)
						best = score;

                    Board[r][c] = savedFrom;
                    Board[tr][tc] = savedTo;
                }
            }
        }
    }

    return best;
}

bool TForm1::IsOpenFile(int col)
{
	for (int r = 0; r < 8; r++)
	{
		if (Board[r][col].Type == Pawn)
			return false;
	}
	return true;
}
void TForm1::SmartMove()
{
	if (IsGameOver) return;
	if (GameMode == 0) return;

	// Если сейчас не ход AI - выходим
	if (CurrentPlayer != Black) return;

	// Структура для хода
	struct ScoredMove {
		int fromRow, fromCol, toRow, toCol;
		int score;
	};

	std::vector<ScoredMove> moves;

	// Собираем все возможные ходы
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (Board[r][c].Color == Black) {
				for (int tr = 0; tr < 8; tr++) {
					for (int tc = 0; tc < 8; tc++) {
						if (IsMoveValid(r, c, tr, tc))  {
							ScoredMove move;

							move.fromRow = r;
							move.fromCol = c;
							move.toRow = tr;
							move.toCol = tc;

							int myScore = EvaluateMove(r, c, tr, tc);

							// временно делаем ход
							TPiece savedFrom = Board[r][c];
							TPiece savedTo   = Board[tr][tc];

							Board[tr][tc] = Board[r][c];
							Board[r][c] = TPiece(Empty, None);

							// лучший ответ белых
							int enemyScore = OpponentBestResponse(White);

							// откатываем
							Board[r][c] = savedFrom;
							Board[tr][tc] = savedTo;

							move.score = myScore - enemyScore;

							moves.push_back(move);
						}
					}
				}
			}
		}
	}

	if (moves.size() == 0) return;

	int index = 0;

	if (GameMode == 1) {
		// ===== ЛЁГКИЙ РЕЖИМ: случайный ход =====
		index = rand() % moves.size();
	} else {
		// ===== СЛОЖНЫЙ РЕЖИМ: лучший ход =====
		int bestScore = moves[0].score;
		for (int i = 1; i < moves.size(); i++) {
			if (moves[i].score > bestScore) {
				bestScore = moves[i].score;
				index = i;
			}
		}
	}

	ScoredMove best = moves[index];

    BotFromRow = best.fromRow;
	BotFromCol = best.fromCol;

	BotToRow = best.toRow;
	BotToCol = best.toCol;

	TPiece movingPiece = Board[best.fromRow][best.fromCol];
	bool capture = Board[best.toRow][best.toCol].Type != None;

	// Выполняем ход
	SelectedX = -1;
	SelectedY = -1;
	ClearPossibleMoves();

	ExecuteMove(best.fromRow, best.fromCol, best.toRow, best.toCol);
	MoveCount++;

	// Меняем игрока (если игра не окончена)
	if (!IsCheckmate && !IsStalemate) {
		CurrentPlayer = (CurrentPlayer == White) ? Black : White;
	}

	UpdateGameState();

	AddMoveToHistory(best.fromRow, best.fromCol, best.toRow, best.toCol,
	movingPiece,capture,CheckFlag);

	// Проверяем окончание игры
	if (CheckGameOver())
	{
		DrawBoard();
		UpdateStatusLabel();
		return;
	}

	DrawBoard();
	UpdateStatusLabel();
}



void __fastcall TForm1::Button3Click(TObject *Sender)
{
	if (IsGameOver)
			return;

	if (GameMode != 0)
	{
		ShowMessage("Компьютер не принимает предложения ничьей.");
		return;
	}

	AnsiString player =
		(CurrentPlayer == White) ? "Белые" : "Черные";

	AnsiString msg =
		player + " предлагают ничью. Согласны?";

	if (Application->MessageBox(
		UnicodeString(msg).c_str(),
		L"Предложение ничьей",
		MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		IsGameOver = true;
		GameTimerRunning = false;
		Timer1->Enabled = false;

		GameOverMessage = "Ничья по соглашению сторон";

		UpdateStatusLabel();
		DrawBoard();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	TStringList *list = new TStringList;

    try
    {
		for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
            {
                if (Board[row][col].Type != None)
                {
                    String pieceName;

                    switch (Board[row][col].Type)
                    {
                        case Pawn:   pieceName = "Pawn"; break;
                        case Knight: pieceName = "Knight"; break;
                        case Bishop: pieceName = "Bishop"; break;
                        case Rook:   pieceName = "Rook"; break;
                        case Queen:  pieceName = "Queen"; break;
                        case King:   pieceName = "King"; break;
                    }

                    String color =
                        (Board[row][col].Color == White)
                        ? "White"
                        : "Black";

                    char fileLetter = 'A' + col;
                    int rank = 8 - row;

                    list->Add(
                        color + " " +
                        pieceName +
                        " : " +
                        String(fileLetter) +
                        IntToStr(rank)
                    );
                }
            }
        }

		if (SaveDialog1->Execute())
		{
			list->SaveToFile(
				SaveDialog1->FileName
			);

			ShowMessage(
				"Позиция сохранена"
			);
		}
    }
    __finally
    {
        delete list;
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button5Click(TObject *Sender)
{
	 if (GameHistory->Count == 0)
		{
			ShowMessage("Нет ходов для сохранения.");
			return;
		}
		if (SaveDialog1->Execute())
		{
			GameHistory->SaveToFile(SaveDialog1->FileName);
			ShowMessage("Статистика сохранена.");
		}
}
//---------------------------------------------------------------------------

