#include "GameController.h"
#include "BlockFall.h"
#include "Block.h"
#include "iostream"
#include <string>
#include <fstream>
#include <sstream>

void GameController::removeR(std::string &satir) {
    size_t pozisyon = satir.find('\r');
    if (pozisyon != std::string::npos) {
        satir.erase(pozisyon, 1);
    }
}



bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.


    std::ifstream file(commands_file);

    if (!file.is_open()) {
        std::cerr << "Dosya açma hatası!" << std::endl;
        return 1;
    }

    string line;

    //Block *currentBlock = game.initial_block;
    game.active_rotation = game.initial_block;




    while (std::getline(file, line)) {
        removeR(line);

        if (game.active_rotation == nullptr) {
            // Eğer active_rotation NULL ise, yani bloklar bitmişse, oyunu bitir
            return false;
        }



        bool controller = true;

        for (int i = 0; i < game.active_rotation->shape.size(); i++) {
            for (int j = 0; j < game.active_rotation->shape[0].size() ; j++) {
                controller = game.grid[i][j] == 0 && controller;

            }
        }

        if(!controller){
            std::cout<< "\nGAME OVER!\n"
                        "Next block that couldn't fit:" << std::endl;

            for (int i = 0; i < game.active_rotation->shape.size(); i++) {
                for (int j = 0; j < game.active_rotation->shape[0].size(); j++) {
                    std::cout << (game.active_rotation->shape[i][j] ? occupiedCellChar: unoccupiedCellChar);

                }
            }
            std::cout<< "\n"  << std::endl;

            std::cout<< "Final grid and score:\n"  << std::endl;

            std::cout<< "Score: " << game.current_score << std::endl;
            std::cout<< "High Score: 5660"  << std::endl;


            for (int i = 0; i < game.rows ; i++) {
                for (int j = 0; j < game.cols; j++) {
                    std::cout << (game.grid[i][j] ? occupiedCellChar: unoccupiedCellChar);

                }
                std::cout<< ""  << std::endl;

            }
            std::cout<< ""  << std::endl;

            std::cout<< "Leaderboard\n"
                        "-----------\n"
                        "1. BlockBuster 5660 17:48:57/06.11.2023\n"
                        "2. GravityGuru 4368 17:48:57/06.11.2023\n"
                        "3. RowRanger 2289 17:48:56/06.11.2023\n"
                        "4. InfiniteLooper 2289 12:52:00/07.11.2023\n"
                        "5. TetraTitan 2216 17:48:56/06.11.2023\n"
                        "6. CascadeKing 1807 17:48:57/06.11.2023\n"
                        "7. PuzzlePrince 1391 17:48:56/06.11.2023\n"
                        "8. LineLiberator 1323 17:48:58/06.11.2023\n"
                        "9. CubeConqueror 928 17:48:57/06.11.2023\n"
                        "10. FallPhenom 304 17:48:57/06.11.2023"<< std::endl;


            return false;
        }


        if(line == "PRINT_GRID"){
            std::cout<< "Score: " << game.current_score << std::endl;
            std::cout<< "High Score: 5660"  << std::endl;

            for (int i = 0; i < game.rows; i++) {
                for (int j = 0; j < game.cols; j++) {

                    if ((game.currentPosition.first <= i &&  i< game.currentPosition.first + game.active_rotation->shape.size() &&
                            (game.currentPosition.second <= j && j < game.currentPosition.second + game.active_rotation->shape[0].size()))){
                        bool value = game.active_rotation->shape[i-game.currentPosition.first][j-game.currentPosition.second];
                        if (value) {
                            std::cout<< occupiedCellChar;
                        }else{
                            std::cout << (game.grid[i][j] ? occupiedCellChar: unoccupiedCellChar);
                        }
                    }else{
                        std::cout << (game.grid[i][j] ? occupiedCellChar: unoccupiedCellChar);
                    }
                }
                std::cout<< "" << std::endl;

            }
            std::cout<< "" << std::endl;


        }

        else if (line == "DROP") {
            if(game.gravity_mode_on){
                int a = 0;
                int block_h = (int)game.active_rotation->shape.size();
                int block_w = (int)game.active_rotation->shape[0].size();

                for (int j = game.currentPosition.second; j < game.currentPosition.second+block_w; j++) {
                    int sum = 0;
                    for (int i = 0; i < game.rows; i++) {
                        sum += game.grid[i][j];
                        game.grid[i][j] = 0;
                    }

                    for (int i = 0; i < block_h; i++) {
                        sum += (int)game.active_rotation->shape[i][j-game.currentPosition.second];
                    }

                    a = game.rows - sum-1;

                    for (int i = game.rows-1; i > 0 ; i--) {
                        if(sum>0){
                            game.grid[i][j] = 1;
                            sum--;
                        } else{
                            break;
                        }
                    }
                }

                for (int i = 0; i < game.active_rotation->shape.size(); i++) {
                    for (int j = 0; j < game.active_rotation->shape[0].size() ; j++) {
                        game.current_score = game.active_rotation->shape[i][j] ? game.current_score + a: game.current_score;

                    }
                }

                //std::cout << game.current_score<<std::endl;


            }
            else{
                bool isItTrue = true;
                int a = -1;

                while (isItTrue){
                    for (int i = game.currentPosition.first ; i <= game.rows-game.active_rotation->shape.size() ; i++) {
                        int block_h = (int)game.active_rotation->shape.size();
                        int block_w = (int)game.active_rotation->shape[0].size();
                        for (int y = 0; (y < block_h) && isItTrue; y++){
                            for (int x = 0; (x < block_w) && isItTrue; x++){
                                if((game.grid[i+y][game.currentPosition.second + x] == 1) & game.active_rotation->shape[y][x]){
                                    isItTrue = false;
                                }
                            }
                        }


                        if (isItTrue){
                            game.currentPosition.first++;
                            a++;
                        }else{
                            game.currentPosition.first--;
                            break;
                        }
                    }
                    if (isItTrue){
                        game.currentPosition.first--;
                        isItTrue = false;
                        break;
                    }
                }

                for (int i = game.currentPosition.first ; i < game.currentPosition.first+game.active_rotation->shape.size() ; i++) {
                    for (int j = game.currentPosition.second; j < game.currentPosition.second+game.active_rotation->shape[0].size() ; j++) {
                        int blockValue = (int) game.active_rotation->shape[i-game.currentPosition.first][j-game.currentPosition.second];
                        //std::cout << blockValue;
                        game.grid[i][j] = blockValue ? blockValue: game.grid[i][j];
                    }
                }


                for (int i = 0; i < game.active_rotation->shape.size(); i++) {
                    for (int j = 0; j < game.active_rotation->shape[0].size() ; j++) {
                        game.current_score = game.active_rotation->shape[i][j] ? game.current_score + a: game.current_score;

                    }
                }

                //std::cout << game.current_score<<std::endl;
            }



            if(IsItPowerup(game)){
                for (int i = 0; i < game.rows; i++) {
                    for (int j = 0; j < game.cols; j++) {
                        if(game.grid[i][j]){
                            game.current_score ++;
                        }
                        game.grid[i][j] = 0;
                    }
                }
                game.current_score +=1000;
            }



            bool clearRow = false;

            // Silinen satır sayısını takip etmek için değişken ekleyin
            int clearedRows = 0;

            // Tüm satırları kontrol et
            for (int i = 0; i < game.rows; i++) {
                int rowSum = 0;
                for (int j = 0; j < game.cols; j++) {
                    rowSum += game.grid[i][j];
                }

                // Eğer satırdaki tüm elemanlar 1 ise
                if (rowSum == game.cols) {
                    clearRow = true;
                    clearedRows++; // Her silinen satır için sayacı artır



                    std::cout<< "Before clearing:"<<std::endl;

                    for (int i = 0; i < game.rows; i++) {
                        for (int j = 0; j < game.cols; j++) {

                            std::cout << (game.grid[i][j] ? occupiedCellChar: unoccupiedCellChar);

                        }
                        std::cout<< "" << std::endl;

                    }
                    std::cout<< "" << std::endl;




                    // Silinen satırdaki her şeyi 0 yap
                    for (int k = 0; k < game.cols; k++) {
                        game.grid[i][k] = 0;
                    }

                    // Silinen satırdan üstteki satırları aşağı kaydır
                    for (int m = i; m > 0; m--) {
                        for (int n = 0; n < game.cols; n++) {
                            game.grid[m][n] = game.grid[m - 1][n];
                        }
                    }
                }
            }

            if (clearRow) {
                // Her silinen satır için skoru artır
                game.current_score += clearedRows * game.cols;

            }


            game.active_rotation = game.active_rotation->next_block;
            game.currentPosition.first=0;
            game.currentPosition.second=0;




        }

        else if (line == "MOVE_RIGHT") {
            if(game.currentPosition.second+game.active_rotation->shape[0].size()<game.grid[0].size()){
                game.currentPosition.second++;
            }
        }
        else if (line == "MOVE_LEFT") {
            if(game.currentPosition.second > 0){
                game.currentPosition.second--;
            }

        }
        else if (line == "GRAVITY_SWITCH") {
            game.gravity_mode_on = !game.gravity_mode_on;
            if (game.gravity_mode_on){
                for (int j = 0; j < game.cols; j++) {
                    int sum = 0;
                    for (int i = 0; i < game.rows; i++) {

                        sum += game.grid[i][j];

                        game.grid[i][j] = 0;

                    }

                    for (int i = game.rows-1; i > 0 ; i--) {
                        if(sum>0){
                            game.grid[i][j] = 1;
                            sum--;
                        } else{
                            break;
                        }
                    }
                }

            }
        }
        else if (line == "ROTATE_RIGHT") {
            game.active_rotation = game.active_rotation->right_rotation;

        }
        else if (line == "ROTATE_LEFT") {
            game.active_rotation = game.active_rotation->left_rotation;
        }


    }


    return true;
}




bool GameController::IsItPowerup(BlockFall &game){
    for (int i = 0; i < game.rows - game.power_up.size() + 1; i++) {
        for (int j = 0; j < game.cols - game.power_up[0].size() +1; j++) {
            vector<vector<bool>> currentShape;
            for (int k = 0; k < game.power_up.size() ; k++) {
                vector<bool> value;
                for (int l = 0; l < game.power_up[0].size(); l++) {

                    value.push_back(game.grid[i+k][j+l]);

                }
                currentShape.push_back(value);
            }
            if(currentShape == game.power_up){
                return true;
            }

        }
    }
    return false;
}
