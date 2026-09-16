//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <pngimage.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------

// рхош
enum PieceType { None, Pawn, Knight, Bishop, Rook, Queen, King };
enum PieceColor { Empty, White, Black };

struct TPiece {
    PieceColor Color;
    PieceType Type;
    TPiece() : Color(Empty), Type(None) {}
    TPiece(PieceColor c, PieceType t) : Color(c), Type(t) {}
};

extern TPiece Board[8][8];
extern PieceColor CurrentPlayer;
extern int SelectedX, SelectedY;
extern bool IsGameOver;
extern bool PossibleMoves[8][8];
extern int GameSeconds;
extern bool GameTimerRunning;

class TForm1 : public TForm
{
__published:
    TImage *Image1;
    TTimer *Timer1;
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Button1;
	TButton *Button2;
	TLabel *Label3;
	TComboBox *ComboBox1;
	TButton *Button3;
	TButton *Button4;
	TSaveDialog *SaveDialog1;
	TButton *Button5;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);

private:
    void SetupBoard();
	void DrawBoard();
	void AddMoveToHistory(int fromRow, int fromCol, int toRow, int toCol, TPiece piece, bool capture, bool check);
    void UpdateStatusLabel();
    void UpdateGameTimerLabel();
    bool IsMoveValid(int fromRow, int fromCol, int toRow, int toCol);
    void ExecuteMove(int fromRow, int fromCol, int toRow, int toCol);
    void CalculatePossibleMoves(int row, int col);
	void ClearPossibleMoves();
    bool IsSquareUnderAttack(int row, int col, PieceColor attacker);
    bool IsCheck(PieceColor kingColor);
	bool HasLegalMoves(PieceColor player);
	AnsiString GameOverMessage;
	bool CheckGameOver();
	void UpdateGameState();
	bool CheckFlag;
	bool IsCheckmate;
	bool IsStalemate;
	int GameMode;
    void SmartMove();
	int GetPieceValue(TPiece piece);
	int EvaluateMove(int fromRow, int fromCol, int toRow, int toCol);
    bool InsufficientMaterial();
	bool IsOpenFile(int col);
	bool CanPieceAttackSquare(int fromRow, int fromCol, int toRow, int toCol);
	int CountAttackers(int row, int col, PieceColor color);
	int AttackValue(int row, int col, PieceColor color);
	int OpponentBestResponse(PieceColor opponent);
	TPngImage *WhiteKingImg;
    TPngImage *WhiteQueenImg;
    TPngImage *WhiteRookImg;
    TPngImage *WhiteBishopImg;
    TPngImage *WhiteKnightImg;
    TPngImage *WhitePawnImg;

    TPngImage *BlackKingImg;
    TPngImage *BlackQueenImg;
    TPngImage *BlackRookImg;
    TPngImage *BlackBishopImg;
    TPngImage *BlackKnightImg;
	TPngImage *BlackPawnImg;

public:
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
