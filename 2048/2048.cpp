/*
 * 檔案名稱:   2048小遊戲
 *
 * 開發人員:   王立翔
 * 未來計畫:   1. 開始遊戲介面
 *             2. 自定義遊戲介面
 *             3. 框架標語更新
 */

# include <iostream>
# include <stdio.h>
# include <string.h>
# include <iomanip>
# include <stdlib.h>
# include <time.h>
# include <conio.h>
# include <windows.h>
using namespace std;

# define ROW 4
# define COL 4
static int score, step;
static int board[ROW][COL] = { { 0, 0, 0, 0 },
                               { 0, 0, 0, 0 },
                               { 0, 0, 0, 0 },
                               { 0, 0, 0, 0 } };
typedef char Statement_str[40];                     // 定義框架字串型別
static Statement_str frame[2*ROW];                  // 遊戲右方框架
enum Status { READY, PLAYING, GAME_OVER, VICTORY }; // 當前遊戲狀態
enum Dir { UP, DOWN, LEFT, RIGHT };                 // 移動方向

void Reset_board();            // 歸零棋盤
int Count_blank();             // 回傳空格數量
int Get_rand_num( int, int );  // 在兩數範圍內取得一隨機數
void Put_piece( int, int );    // 將指定棋子數值放進第n個空格
void Clear_screen();           // 清空螢幕
void Delay( int );             // 程式延遲運作ms毫秒( 1000毫秒 = 1秒 )

void Init_game();              // 初始化遊戲，隨機兩格放2，分數歸零
void Rand_put_piece( int );    // 隨機在棋盤上一空格放入指定棋子值
Dir Get_dir();                 // 輸入移動方向指令
void Print_game();             // 輸出遊戲
void Set_frame( Status );      // 依照遊戲狀態，設定遊戲右方框架內容

bool IsGameOver();   // 棋子無合法移動路徑
bool IsVictory();    // 達成某一方格2048

bool Move_up();      // 上、下、左、右移動指令
bool Move_down();    // 若成功使任一顆棋子移動回傳true
bool Move_left();    // 若無棋子被移動，視為無效移動
bool Move_right();   // 回傳false

int main() {
  bool isExit = 0;
  Dir dir;

  while ( !isExit ) {
    // Clear_screen();
    Init_game();
    Print_game();

    // 若棋盤上無空格就檢查是否game over，沒game over就繼續遊戲
    while ( Count_blank() != 0 || !IsGameOver() ) {
      bool moveFail;
      do {
        dir = Get_dir();
        switch ( dir ) {
          case UP:
            moveFail = !Move_up();
            break;
          case DOWN:
            moveFail = !Move_down();
            break;
          case LEFT:
            moveFail = !Move_left();
            break;
          case RIGHT:
            moveFail = !Move_right();
            break;
        } // switch
      } while ( moveFail );
      step++;

      Rand_put_piece( 2 );
      Clear_screen();
      Set_frame( PLAYING );
      Print_game();
    } // while

    Clear_screen();
    if ( IsVictory() )
      Set_frame( VICTORY );
    else
      Set_frame( GAME_OVER );
    Print_game();

    char ch;
    cout << "Do you want to play it again? (Y)or(N)" << endl;
    do {
        ch = getch();
        switch ( ch ) {
          case 'y':
          case 'Y':
            isExit = false;
            break;
          case 'n':
          case 'N':
            isExit = true;
            break;
        } // switch
    } while ( ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N' );
    Clear_screen();
  } // while

  cout << "Thank you for playing, see you!" << endl;
  system( "pause" );
  return 0;
} // main()

void Print_game() {
  int indexOfFrame = 0;
  int formWidth = 5;          // 格子寬度
  char interval[5] = "    ";  // 棋盤與右方框架間隔空格

  // 最上列牆壁 "┌────┬────┬────┬────┐"
  cout << "┌";
  for ( int i = 0 ; i < COL ; i++ ) {         // 共COL格
    for ( int c = 1 ; c <= formWidth ; c++ )  // 格子寬度
      cout << "─";

    if ( i < COL - 1 ) cout << "┬";
  } cout << "┐";

  // 第一列框架
  cout << interval << frame[ indexOfFrame++ ] << endl;


  for ( int i = 0 ; i < ROW ; i++ ) {             // 一列中間格子+一列中間牆壁，共ROW組

    // 中間格子 "│    │    │    │    │"
    for ( int j = 0 ; j < COL ; j++ ) {           // 一左牆壁+一棋子，共COL組
      cout << "│";
      if ( board[i][j] > 0 )
        cout << setw( formWidth ) << board[i][j]; // 棋子
      else
        cout << setw( formWidth ) << "";          // 空棋子
    } cout << "│";

    // 中間列框架
    cout << interval << frame[ indexOfFrame++ ] << endl;


    // 中間牆壁 "├────┼────┼────┼────┤"
    if ( i < ROW - 1 ) {
      cout << "├";
      for ( int i = 0 ; i < COL ; i++ ) {         // 共COL格
        for ( int c = 1 ; c <= formWidth ; c++ )  // 格子寬度
          cout << "─";

        if ( i < COL - 1 ) cout << "┼";
      } cout << "┤";

      // 中間列框架
      cout << interval << frame[ indexOfFrame++ ] << endl;
    } // if

  } // for


  // 最下列牆壁 "└────┴────┴────┴────┘"
  cout << "└";
  for ( int i = 0 ; i < COL ; i++ ) {         // 共COL格
    for ( int c = 1 ; c <= formWidth ; c++ )  // 格子寬度
      cout << "─";

    if ( i < COL - 1 ) cout << "┴";
  } cout << "┘";

  // 最下列框架
  cout << interval << frame[ indexOfFrame++ ] << endl;

} // Print_game()

void Set_frame( Status game_status ) {
  strcpy( frame[0], "┌────────────────┐" );     // 最上列牆壁
  strcpy( frame[2*ROW], "└────────────────┘");  // 最下列牆壁
  for ( int i = 1 ; i < 2 * ROW ; i++ ) {       // 左、右側牆壁
    strcpy( frame[i], "│                │" );
  } // for

  sprintf( frame[2*ROW-2], "│ 目前分數:%-6d│", score );  // 倒數第三列分數欄
  sprintf( frame[2*ROW-1], "│ 步數:%-10d│", step );      // 倒數第二列步數欄

  if ( game_status == READY ) {
    strcpy( frame[ROW-1], "│     ↑         │" );
    strcpy( frame[ROW], "│ 按←↓→來開始 │" );
  }
  else if ( game_status == GAME_OVER ) {
    strcpy( frame[ROW-1], "│    ＧＡＭＥ    │" );
    strcpy( frame[ROW], "│    ＯＶＥＲ    │" );
  }
  else if ( game_status == VICTORY ) {
    strcpy( frame[ROW-1], "│ ＶＩＣＴＯＲＹ │" );
    strcpy( frame[ROW], "│　 　！！！　 　│" );
  }
  else {
    strcpy( frame[ROW-1], "│    ２０２０    │" );
    strcpy( frame[ROW], "│    ２０４８    │" );
  }
} // Set_frame();

void Reset_board() {
  for ( int i = 0 ; i < ROW ; i++ ) {
    for ( int j = 0 ; j < COL ; j++ ) {
      board[i][j] = 0;
    }
  } // for
} // Reset_board()

int Count_blank() {
  int numOfBlank = 0;
  for ( int i = 0 ; i < ROW ; i++ ) {
    for ( int j = 0 ; j < COL ; j++ ) {
      if ( board[i][j] == 0 )
        numOfBlank++;
    }
  } // for

  return numOfBlank;
} // Count_blank()

int Get_rand_num( int rand_min, int rand_max ) {
  // 函式引數錯誤
  if ( rand_min > rand_max ) {
    cout << "Get_rand_num() value error" << endl;
    exit( 1 );
  } // if

  srand( time( NULL ) );
  int rand_num, arrayOfRand[100];
  for ( int i = 0 ; i < 100 ; i++ ) {
    arrayOfRand[i] = rand() % ( rand_max + 1 - rand_min ) + rand_min;
  } // for

  rand_num = arrayOfRand[ rand() % 100 ];

  // 檢測數值生成範圍錯誤
  if ( rand_num < rand_min || rand_num > rand_max ) {
    cout << "Get_rand_num() rand error" << endl;
    exit( 1 );
  } // if

  return rand_num;
} // Get_rand_num()

void Put_piece( int piece_val, int n ) {
  int pace = 0;
  for ( int i = 0 ; i < ROW ; i++ ) {
    for ( int j = 0 ; j < COL ; j++ ) {
      if ( board[i][j] == 0 ) pace++;
      if ( pace == n ) {
        board[i][j] = piece_val;
        return;
      }
    }
  } // for

  // 空格數不足
  if ( pace < n ) {
    cout << "Put_piece() blank doesn't enough error" << endl;
    exit( 1 );
  } // if
} // Put_piece()

void Clear_screen() {
  system( "cls" );
} // Clear_screen();

void Delay( int ms ) {
  Sleep( ms );
} // Delay()

void Rand_put_piece( int piece_val ) {
  int numOfBlank = Count_blank();
  int n = Get_rand_num( 1, numOfBlank );
  Put_piece( piece_val, n );
} // Rand_put_piece()

void Init_game() {
  score = 0;
  step = 0;
  Reset_board();
  Rand_put_piece( 2 );
  Rand_put_piece( 2 );
  Set_frame( READY );
} // Init_game()

Dir Get_dir() {
  int ch1, ch2;

  while ( true ) {
    ch1 = getch();
    switch ( ch1 ) {
      case 'w': case 'W': case '5':
        return UP;
      case 's': case 'S': case '2':
        return DOWN;
      case 'a': case 'A': case '1':
        return LEFT;
      case 'd': case 'D': case '3':
        return RIGHT;
    } // switch

    if ( ch1 == 224 ) { // 方向鍵
      ch2 = getch();
      switch ( ch2 ) {
        case 72:
          return UP;
        case 80:
          return DOWN;
        case 75:
          return LEFT;
        case 77:
          return RIGHT;
      }
    } // if

  } // while 無窮迴圈
} // Get_dir()

bool IsGameOver() {
  if ( Count_blank() > 0 ) // 仍有空格
    return false;
  // 檢查橫列是否存在左右相等
  for ( int i = 0 ; i < ROW ; i++ ) {
    for ( int j = 1 ; j < COL ; j++ ) {
      if ( board[i][j] == board[i][j-1] )
        return false;
    }
  }
  // 檢查直行是否存在上下相等
  for ( int j = 0 ; j < COL ; j++ ) {
    for ( int i = 1 ; i < ROW ; i++ ) {
      if ( board[i][j] == board[i-1][j] )
        return false;
    }
  }

  return true; // 無相同數值棋子緊鄰
} // IsGameOver()

bool IsVictory() {
  for ( int i = 0 ; i < ROW ; i++ ) {
    for ( int j = 0 ; j < COL ; j++ ) {
      if ( board[i][j] >= 2048 )
        return true;
    }
  } // for

  return false;
} // IsVictory()

bool Move_up() {
  bool moveFail = true;
  for ( int col = 0 ; col < COL ; col++ ) {
    int last = -1, j;
    for ( int i = 0 ; i < ROW ; i++ ) {   // 由上至下
      if ( board[i][col] > 0 ) {   // 碰到棋子
        if ( last == board[i][col] ) {   // 兩個棋子可以合併
          j = i - 1;
          while ( j > 0 && board[j][col] != last ) j--;  // 往回找上個棋子位置
          // 合併棋子
          board[j][col] += board[i][col];
          board[i][col] = 0;
          score += board[j][col];

          last = -1;
          moveFail = false; // 成功移動棋子
        }
        else if ( last != board[i][col] ) {
          j = i;
          while ( j > 0 && board[j-1][col] == 0 ) j--;   // 往回找上個空格位置
          // 將棋子放至該空格
          if ( j != i ) {
            int temp = board[j][col];
            board[j][col] = board[i][col];
            board[i][col] = temp;

            moveFail = false; // 成功移動棋子
          } // if

          last = board[j][col];
        }
      } // if 碰到棋子
    } // for 由上至下
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_up()

bool Move_down() {
  bool moveFail = true;
  for ( int col = 0 ; col < COL ; col++ ) {
    int last = -1, j;
    for ( int i = ROW - 1 ; i >= 0 ; i-- ) {   // 由下至上
      if ( board[i][col] > 0 ) {   // 碰到棋子
        if ( last == board[i][col] ) {   // 兩個棋子可以合併
          j = i + 1;
          while ( j < ROW - 1 && board[j][col] != last ) j++;  // 往回找上個棋子位置
          // 合併棋子
          board[j][col] += board[i][col];
          board[i][col] = 0;
          score += board[j][col];

          last = -1;
          moveFail = false; // 成功移動棋子
        }
        else if ( last != board[i][col] ) {
          j = i;
          while ( j < ROW - 1 && board[j+1][col] == 0 ) j++;   // 往回找上個空格位置
          // 將棋子放至該空格
          if ( j != i ) {
            int temp = board[j][col];
            board[j][col] = board[i][col];
            board[i][col] = temp;

            moveFail = false; // 成功移動棋子
          } // if

          last = board[j][col];
        }
      } // if 碰到棋子
    } // for 由下至上
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_down()

bool Move_left() {
  bool moveFail = true;
  for ( int row = 0 ; row < ROW; row++ ) {
    int last = -1, j;
    for ( int i = 0 ; i < COL ; i++ ) {   // 由左至右
      if ( board[row][i] > 0 ) {   // 碰到棋子
        if ( last == board[row][i] ) {   // 兩個棋子可以合併
          j = i - 1;
          while ( j > 0 && board[row][j] != last ) j--;  // 往回找上個棋子位置
          // 合併棋子
          board[row][j] += board[row][i];
          board[row][i] = 0;
          score += board[row][j];

          last = -1;
          moveFail = false; // 成功移動棋子
        }
        else if ( last != board[row][i] ) {
          j = i;
          while ( j > 0 && board[row][j-1] == 0 ) j--;   // 往回找上個空格位置
          // 將棋子放至該空格
          if ( j != i ) {
            int temp = board[row][j];
            board[row][j] = board[row][i];
            board[row][i] = temp;

            moveFail = false; // 成功移動棋子
          } // if

          last = board[row][j];
        }
      } // if 碰到棋子
    } // for 由左至右
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_left()

bool Move_right() {
  bool moveFail = true;
  for ( int row = 0 ; row < ROW; row++ ) {
    int last = -1, j;
    for ( int i = COL - 1 ; i >= 0 ; i-- ) {   // 由右至左
      if ( board[row][i] > 0 ) {   // 碰到棋子
        if ( last == board[row][i] ) {   // 兩個棋子可以合併
          j = i + 1;
          while ( j < COL - 1 && board[row][j] != last ) j++;  // 往回找上個棋子位置
          // 合併棋子
          board[row][j] += board[row][i];
          board[row][i] = 0;
          score += board[row][j];

          last = -1;
          moveFail = false; // 成功移動棋子
        }
        else if ( last != board[row][i] ) {
          j = i;
          while ( j < COL - 1 && board[row][j+1] == 0 ) j++;   // 往回找上個空格位置
          // 將棋子放至該空格
          if ( j != i ) {
            int temp = board[row][j];
            board[row][j] = board[row][i];
            board[row][i] = temp;

            moveFail = false; // 成功移動棋子
          } // if

          last = board[row][j];
        }
      } // if 碰到棋子
    } // for 由右至左
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_right()

