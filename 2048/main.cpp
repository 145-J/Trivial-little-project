#include <iostream>
#include <vector>
#include <random>   
#include <ctime> 
#include <cstdlib>
#include <windows.h>
#define KUAN 4
std::vector<std::vector<int>> board(KUAN, std::vector<int>(KUAN, 0));
int score = 0;

void rotate_clockwise() {

    std::vector<std::vector<int>> temp_board(KUAN, std::vector<int>(KUAN, 0));
    for (int r = 0; r < KUAN; ++r) {
        for (int c = 0; c < KUAN; ++c) {
            temp_board[c][KUAN - 1 - r] = board[r][c];
        }
    }
    board = temp_board;
}
void rotate_counter_clockwise() {
    std::vector<std::vector<int>> temp_board(KUAN, std::vector<int>(KUAN, 0));
    for (int r = 0; r < KUAN; ++r) {
        for (int c = 0; c < KUAN; ++c) {
            temp_board[KUAN - 1 - c][r] = board[r][c];
        }
    }
    board = temp_board;
}
void hebing_lift() {
    for (int i = 0; i < 4; i++) {
        for (int b_ptr = 0; b_ptr < KUAN-1; b_ptr++) {
            if(board[i][b_ptr] == board[i][b_ptr+1]) {
                score += board[i][b_ptr];
                board[i][b_ptr] *= 2; 
                board[i][b_ptr+1] =  0;
            }
        }
    }
}
void mov_lift() {
    
    for (int i = 0; i < 4; i++) {
        int slow_ptr = 0;
        for (int fast_ptr = 0; fast_ptr < KUAN; fast_ptr++) {
            if (board[i][fast_ptr] != 0) {
                board[i][slow_ptr] = board[i][fast_ptr];
                slow_ptr++;
            }
        }
        while (slow_ptr < KUAN) {
            board[i][slow_ptr] = 0;
            slow_ptr++;
        }
    }
}
void hebing_right() {
    for (int i = 0; i < 4; i++) {
        for (int b_ptr = KUAN - 1; b_ptr > 0; b_ptr--) {
            if(board[i][b_ptr] == board[i][b_ptr-1]) {
                score += board[i][b_ptr-1];
                board[i][b_ptr-1] *= 2; 
                board[i][b_ptr] =  0;
            }
        }
    }
}
void mov_right() {
    
    for (int i = 0; i < 4; i++) {
        int slow_ptr = KUAN - 1;
        for (int fast_ptr = KUAN - 1; fast_ptr >= 0; fast_ptr--) {
            if (board[i][fast_ptr] != 0) {
                board[i][slow_ptr] = board[i][fast_ptr];
                slow_ptr--;
            }
        }
        while (slow_ptr >= 0) {
            board[i][slow_ptr] = 0;
            slow_ptr--;
        }
    }
}
void mov_up(){
    rotate_clockwise();
    mov_right();
    hebing_right();
    mov_right();
    rotate_counter_clockwise();
}
void mov_down(){
    rotate_clockwise();
    mov_lift();
    hebing_lift();
    mov_lift();
    rotate_counter_clockwise();
}
void printt () {
    for (int j = 0; j < KUAN; j++) {
        for (int i = 0; i < KUAN; i++) {
            std::cout << board[j][i];
        }
    }
    std::cout << std::endl;

}
void add_new_tile() {
    std::vector<std::pair<int, int>> empty_cells;
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c) {
            if (board[r][c] == 0) {
                empty_cells.push_back({ r, c });
            }
        }
    }
    if (empty_cells.empty()) {
        return;
    }
    static std::mt19937 generator(static_cast<unsigned int>(time(0)));
    std::uniform_int_distribution<int> distribution(0, empty_cells.size() - 1);
    int random_index = distribution(generator);
    auto chosen_cell = empty_cells[random_index];
    int row = chosen_cell.first;
    int col = chosen_cell.second;
    std::uniform_int_distribution<int> value_distribution(1, 10);
    int random_value = value_distribution(generator);
    int new_tile_value = (random_value <= 9) ? 2 : 4; 
    board[row][col] = new_tile_value;
}
void draw_empty_grid() {
    std::cout << "+------" << "+------" << "+------" << "+------+" << std::endl;
    std::cout << "|      " << "|      " << "|      " << "|      |" << std::endl;
    std::cout << "+------" << "+------" << "+------" << "+------+" << std::endl;
    std::cout << "|      " << "|      " << "|      " << "|      |" << std::endl;
    std::cout << "+------" << "+------" << "+------" << "+------+" << std::endl;
    std::cout << "|      " << "|      " << "|      " << "|      |" << std::endl;
    std::cout << "+------" << "+------" << "+------" << "+------+" << std::endl;
    std::cout << "|      " << "|      " << "|      " << "|      |" << std::endl;
    std::cout << "+------" << "+------" << "+------" << "+------+" << std::endl;
}
void board_to_screen(int r, int c, int& screen_x, int& screen_y) {
    screen_x = c * 7 + 1 ;
    screen_y = r * (1 + 1) + 1; // ��Ϊ r * 3 + 1
}
bool is_game_over() {
    // 1. 检查是否还有空格，如果有，游戏肯定没结束
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c) {
            if (board[r][c] == 0) {
                return false; // 找到空格，游戏继续
            }
        }
    }

    // 2. 如果没有空格了，检查是否还能合并
    // 遍历所有格子，检查它和右边、下边的邻居是否相同
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board[r].size(); ++c) {
            int current = board[r][c];
            // 检查右边
            if (c < board[r].size() - 1 && current == board[r][c + 1]) {
                return false; // 可以向右合并
            }
            // 检查下边
            if (r < board.size() - 1 && current == board[r + 1][c]) {
                return false; // 可以下合并
            }
        }
    }

    // 如果既没有空格，也无法合并，游戏结束
    return true;
}
void set_cursor_position(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void update_cell() {
    int screen_x, screen_y;
    for (int r = 0; r < KUAN; ++r) {
        for (int c = 0; c < KUAN; ++c) {
            board_to_screen(r, c, screen_x, screen_y);
            set_cursor_position(screen_x, screen_y);

            if (board[r][c] == 0) {
                std::cout << "      ";
            }
            else {
                switch(board[r][c]) {
                    case 2:   set_color(6); break;    // 黄色
                    case 4:   set_color(14); break;   // 亮黄色
                    case 8:   set_color(12); break;   // 红色
                    case 16:  set_color(13); break;   // 紫色
                    case 32:  set_color(10); break;   // 亮绿色
                    case 64:  set_color(11); break;   // 亮青色
                    case 128: set_color(9); break;    // 亮蓝色
                    case 256: set_color(5); break;    // 洋红色
                    case 512: set_color(3); break;    // 青色
                    case 1024:set_color(2); break;    // 绿色
                    case 2048:set_color(4); break;    // 蓝色
                    default:  set_color(15); break;   // 白色（用于更大的数字）
                }
                std::cout.width(6);
                std::cout << board[r][c];
            }
            set_cursor_position(0,11);
            set_color(15);
            std::cout<< "score: "<< score <<std::endl;
        }
    }
}
void handle_input() {
    auto board_before_move = board; 
    INPUT_RECORD irInBuf[128];
    DWORD cNumRead;
    GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &cNumRead);
    if (cNumRead > 0) {
        ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), irInBuf, 128, &cNumRead);
        for (DWORD i = 0; i < cNumRead; i++) {
            if (irInBuf[i].EventType == KEY_EVENT && irInBuf[i].Event.KeyEvent.bKeyDown) {
                WORD keyCode = irInBuf[i].Event.KeyEvent.wVirtualKeyCode;
                
                switch (keyCode) {
                    case VK_UP:   // 方向键上
                        mov_up();
                        if (board != board_before_move) {
                            add_new_tile();
                            update_cell();
                        } 
                        break;
                        
                    case VK_DOWN: // 方向键下
                        mov_down();
                        if (board != board_before_move){
                            add_new_tile();
                            update_cell();
                        }
                        break;
                        
                    case VK_LEFT: // 方向键左
                        mov_lift();
                        hebing_lift();
                        mov_lift();
                        if (board != board_before_move) {
                            
                            add_new_tile();
                            update_cell();
                        }
                        break;
                        
                    case VK_RIGHT:// 方向键右
                        mov_right();
                        hebing_right();
                        mov_right();
                        if (board != board_before_move){
                            
                            add_new_tile();
                            update_cell();
                        }
                        break;
                }
            }
        }
    }
}
bool ask_to_quit() {
    char choice;
    while (true) {
        set_color(7);
        std::cout << "Do you want to exit? (y/n): ";
        std::cin >> choice;

        // 使用 tolower 将输入转换为小写，这样 'Y' 和 'y' 都能被识别
        choice = tolower(choice);

        if (choice == 'y') {
            return true; // 用户选择退出
        } else if (choice == 'n') {
            return false; // 用户选择不退出（例如，重新开始）
        } else {
            // 如果输入无效，提示用户并继续循环
            std::cout << "无效输入，请输入 'y' 或 'n'。" << std::endl;
        }
    }
}
void reset_game() {
    // 1. 重置棋盘为全0
    board.assign(KUAN, std::vector<int>(KUAN, 0));
    // 2. 重置分数 (如果你有分数变量)
    score = 0; 
    draw_empty_grid();
    add_new_tile();
    update_cell();
}
int main(){
    bool want_to_quit = false;
    while(!want_to_quit){
        reset_game();
        bool game_over = false;
        while(!game_over){
            handle_input();
            game_over = is_game_over();
            if (game_over){
                set_color(12);
                std::cout<< "GAME OVER!" <<std::endl;
                if (ask_to_quit()) {
                    want_to_quit = true;
                    break;
                }else{
                    system("cls");
                }
            }
        }
    }
    return 0;
}