/*
 * �ɮצW��:   2048�p�C��
 *
 * �}�o�H��:   ���ߵ�
 * ���ӭp�e:   1. �}�l�C������
 *             2. �۩w�q�C������
 *             3. �ج[�лy��s
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
typedef char Statement_str[40];                     // �w�q�ج[�r�ꫬ�O
static Statement_str frame[2*ROW];                  // �C���k��ج[
enum Status { READY, PLAYING, GAME_OVER, VICTORY }; // ��e�C�����A
enum Dir { UP, DOWN, LEFT, RIGHT };                 // ���ʤ�V

void Reset_board();            // �k�s�ѽL
int Count_blank();             // �^�ǪŮ�ƶq
int Get_rand_num( int, int );  // �b��ƽd�򤺨��o�@�H����
void Put_piece( int, int );    // �N���w�Ѥl�ƭȩ�i��n�ӪŮ�
void Clear_screen();           // �M�ſù�
void Delay( int );             // �{������B�@ms�@��( 1000�@�� = 1�� )

void Init_game();              // ��l�ƹC���A�H������2�A�����k�s
void Rand_put_piece( int );    // �H���b�ѽL�W�@�Ů��J���w�Ѥl��
Dir Get_dir();                 // ��J���ʤ�V���O
void Print_game();             // ��X�C��
void Set_frame( Status );      // �̷ӹC�����A�A�]�w�C���k��ج[���e

bool IsGameOver();   // �Ѥl�L�X�k���ʸ��|
bool IsVictory();    // �F���Y�@���2048

bool Move_up();      // �W�B�U�B���B�k���ʫ��O
bool Move_down();    // �Y���\�ϥ��@���Ѥl���ʦ^��true
bool Move_left();    // �Y�L�Ѥl�Q���ʡA�����L�Ĳ���
bool Move_right();   // �^��false

int main() {
  bool isExit = 0;
  Dir dir;

  while ( !isExit ) {
    // Clear_screen();
    Init_game();
    Print_game();

    // �Y�ѽL�W�L�Ů�N�ˬd�O�_game over�A�Sgame over�N�~��C��
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
  int formWidth = 5;          // ��l�e��
  char interval[5] = "    ";  // �ѽL�P�k��ج[���j�Ů�

  // �̤W�C��� "�z�w�w�w�w�s�w�w�w�w�s�w�w�w�w�s�w�w�w�w�{"
  cout << "�z";
  for ( int i = 0 ; i < COL ; i++ ) {         // �@COL��
    for ( int c = 1 ; c <= formWidth ; c++ )  // ��l�e��
      cout << "�w";

    if ( i < COL - 1 ) cout << "�s";
  } cout << "�{";

  // �Ĥ@�C�ج[
  cout << interval << frame[ indexOfFrame++ ] << endl;


  for ( int i = 0 ; i < ROW ; i++ ) {             // �@�C������l+�@�C��������A�@ROW��

    // ������l "�x    �x    �x    �x    �x"
    for ( int j = 0 ; j < COL ; j++ ) {           // �@�����+�@�Ѥl�A�@COL��
      cout << "�x";
      if ( board[i][j] > 0 )
        cout << setw( formWidth ) << board[i][j]; // �Ѥl
      else
        cout << setw( formWidth ) << "";          // �ŴѤl
    } cout << "�x";

    // �����C�ج[
    cout << interval << frame[ indexOfFrame++ ] << endl;


    // ������� "�u�w�w�w�w�q�w�w�w�w�q�w�w�w�w�q�w�w�w�w�t"
    if ( i < ROW - 1 ) {
      cout << "�u";
      for ( int i = 0 ; i < COL ; i++ ) {         // �@COL��
        for ( int c = 1 ; c <= formWidth ; c++ )  // ��l�e��
          cout << "�w";

        if ( i < COL - 1 ) cout << "�q";
      } cout << "�t";

      // �����C�ج[
      cout << interval << frame[ indexOfFrame++ ] << endl;
    } // if

  } // for


  // �̤U�C��� "�|�w�w�w�w�r�w�w�w�w�r�w�w�w�w�r�w�w�w�w�}"
  cout << "�|";
  for ( int i = 0 ; i < COL ; i++ ) {         // �@COL��
    for ( int c = 1 ; c <= formWidth ; c++ )  // ��l�e��
      cout << "�w";

    if ( i < COL - 1 ) cout << "�r";
  } cout << "�}";

  // �̤U�C�ج[
  cout << interval << frame[ indexOfFrame++ ] << endl;

} // Print_game()

void Set_frame( Status game_status ) {
  strcpy( frame[0], "�z�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{" );     // �̤W�C���
  strcpy( frame[2*ROW], "�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}");  // �̤U�C���
  for ( int i = 1 ; i < 2 * ROW ; i++ ) {       // ���B�k�����
    strcpy( frame[i], "�x                �x" );
  } // for

  sprintf( frame[2*ROW-2], "�x �ثe����:%-6d�x", score );  // �˼ƲĤT�C������
  sprintf( frame[2*ROW-1], "�x �B��:%-10d�x", step );      // �˼ƲĤG�C�B����

  if ( game_status == READY ) {
    strcpy( frame[ROW-1], "�x     ��         �x" );
    strcpy( frame[ROW], "�x ���������Ӷ}�l �x" );
  }
  else if ( game_status == GAME_OVER ) {
    strcpy( frame[ROW-1], "�x    �բϢۢ�    �x" );
    strcpy( frame[ROW], "�x    �ݢ�Ӣ�    �x" );
  }
  else if ( game_status == VICTORY ) {
    strcpy( frame[ROW-1], "�x ��עѢ�ݢ�� �x" );
    strcpy( frame[ROW], "�x�@ �@�I�I�I�@ �@�x" );
  }
  else {
    strcpy( frame[ROW-1], "�x    ��������    �x" );
    strcpy( frame[ROW], "�x    ��������    �x" );
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
  // �禡�޼ƿ��~
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

  // �˴��ƭȥͦ��d����~
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

  // �Ů�Ƥ���
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

    if ( ch1 == 224 ) { // ��V��
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

  } // while �L�a�j��
} // Get_dir()

bool IsGameOver() {
  if ( Count_blank() > 0 ) // �����Ů�
    return false;
  // �ˬd��C�O�_�s�b���k�۵�
  for ( int i = 0 ; i < ROW ; i++ ) {
    for ( int j = 1 ; j < COL ; j++ ) {
      if ( board[i][j] == board[i][j-1] )
        return false;
    }
  }
  // �ˬd����O�_�s�b�W�U�۵�
  for ( int j = 0 ; j < COL ; j++ ) {
    for ( int i = 1 ; i < ROW ; i++ ) {
      if ( board[i][j] == board[i-1][j] )
        return false;
    }
  }

  return true; // �L�ۦP�ƭȴѤl��F
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
    for ( int i = 0 ; i < ROW ; i++ ) {   // �ѤW�ܤU
      if ( board[i][col] > 0 ) {   // �I��Ѥl
        if ( last == board[i][col] ) {   // ��ӴѤl�i�H�X��
          j = i - 1;
          while ( j > 0 && board[j][col] != last ) j--;  // ���^��W�ӴѤl��m
          // �X�ִѤl
          board[j][col] += board[i][col];
          board[i][col] = 0;
          score += board[j][col];

          last = -1;
          moveFail = false; // ���\���ʴѤl
        }
        else if ( last != board[i][col] ) {
          j = i;
          while ( j > 0 && board[j-1][col] == 0 ) j--;   // ���^��W�ӪŮ��m
          // �N�Ѥl��ܸӪŮ�
          if ( j != i ) {
            int temp = board[j][col];
            board[j][col] = board[i][col];
            board[i][col] = temp;

            moveFail = false; // ���\���ʴѤl
          } // if

          last = board[j][col];
        }
      } // if �I��Ѥl
    } // for �ѤW�ܤU
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_up()

bool Move_down() {
  bool moveFail = true;
  for ( int col = 0 ; col < COL ; col++ ) {
    int last = -1, j;
    for ( int i = ROW - 1 ; i >= 0 ; i-- ) {   // �ѤU�ܤW
      if ( board[i][col] > 0 ) {   // �I��Ѥl
        if ( last == board[i][col] ) {   // ��ӴѤl�i�H�X��
          j = i + 1;
          while ( j < ROW - 1 && board[j][col] != last ) j++;  // ���^��W�ӴѤl��m
          // �X�ִѤl
          board[j][col] += board[i][col];
          board[i][col] = 0;
          score += board[j][col];

          last = -1;
          moveFail = false; // ���\���ʴѤl
        }
        else if ( last != board[i][col] ) {
          j = i;
          while ( j < ROW - 1 && board[j+1][col] == 0 ) j++;   // ���^��W�ӪŮ��m
          // �N�Ѥl��ܸӪŮ�
          if ( j != i ) {
            int temp = board[j][col];
            board[j][col] = board[i][col];
            board[i][col] = temp;

            moveFail = false; // ���\���ʴѤl
          } // if

          last = board[j][col];
        }
      } // if �I��Ѥl
    } // for �ѤU�ܤW
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_down()

bool Move_left() {
  bool moveFail = true;
  for ( int row = 0 ; row < ROW; row++ ) {
    int last = -1, j;
    for ( int i = 0 ; i < COL ; i++ ) {   // �ѥ��ܥk
      if ( board[row][i] > 0 ) {   // �I��Ѥl
        if ( last == board[row][i] ) {   // ��ӴѤl�i�H�X��
          j = i - 1;
          while ( j > 0 && board[row][j] != last ) j--;  // ���^��W�ӴѤl��m
          // �X�ִѤl
          board[row][j] += board[row][i];
          board[row][i] = 0;
          score += board[row][j];

          last = -1;
          moveFail = false; // ���\���ʴѤl
        }
        else if ( last != board[row][i] ) {
          j = i;
          while ( j > 0 && board[row][j-1] == 0 ) j--;   // ���^��W�ӪŮ��m
          // �N�Ѥl��ܸӪŮ�
          if ( j != i ) {
            int temp = board[row][j];
            board[row][j] = board[row][i];
            board[row][i] = temp;

            moveFail = false; // ���\���ʴѤl
          } // if

          last = board[row][j];
        }
      } // if �I��Ѥl
    } // for �ѥ��ܥk
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_left()

bool Move_right() {
  bool moveFail = true;
  for ( int row = 0 ; row < ROW; row++ ) {
    int last = -1, j;
    for ( int i = COL - 1 ; i >= 0 ; i-- ) {   // �ѥk�ܥ�
      if ( board[row][i] > 0 ) {   // �I��Ѥl
        if ( last == board[row][i] ) {   // ��ӴѤl�i�H�X��
          j = i + 1;
          while ( j < COL - 1 && board[row][j] != last ) j++;  // ���^��W�ӴѤl��m
          // �X�ִѤl
          board[row][j] += board[row][i];
          board[row][i] = 0;
          score += board[row][j];

          last = -1;
          moveFail = false; // ���\���ʴѤl
        }
        else if ( last != board[row][i] ) {
          j = i;
          while ( j < COL - 1 && board[row][j+1] == 0 ) j++;   // ���^��W�ӪŮ��m
          // �N�Ѥl��ܸӪŮ�
          if ( j != i ) {
            int temp = board[row][j];
            board[row][j] = board[row][i];
            board[row][i] = temp;

            moveFail = false; // ���\���ʴѤl
          } // if

          last = board[row][j];
        }
      } // if �I��Ѥl
    } // for �ѥk�ܥ�
  } // for

  if ( ! moveFail ) return true;
  return false;
} // Move_right()

