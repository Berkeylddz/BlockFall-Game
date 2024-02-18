#include "BlockFall.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>


BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}


void removeR(std::string &satir) {
    size_t pozisyon = satir.find('\r');
    if (pozisyon != std::string::npos) {
        satir.erase(pozisyon, 1);
    }
}

void BlockFall::rotation(Block *&block){
    Block *head = block;
    Block *right;
    while (true){
        right = rotateClockwise(block);

        if(right->shape != head->shape){
            block->right_rotation = right;
            right->left_rotation = block;
            right->next_block = block->next_block;
        }
        else{
            block->right_rotation = head;
            head->left_rotation = block;
            block->next_block = head->next_block;
            delete right;
            break;
        }
        block = block->right_rotation;

    }

}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every rotationBlock, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last rotationBlock from the input file (do not add it to the linked list!)

    std::ifstream file(input_file);



    if(file.is_open()){
        bool initIsDefined = false;

        initial_block = new Block();
        Block* currentBlock = initial_block;

        std::string line;
        while (std::getline(file,line)){
            removeR(line);

            std::istringstream iss(line);
            std::vector<bool> row;

            for(char character : line){
                if(std::isdigit(character)){
                    row.push_back(character=='1');

                }
            }

            if(line.find('[') != std::string::npos){
                if(initIsDefined){
                    currentBlock->next_block = new Block();
                    currentBlock = currentBlock->next_block;
                }
            } else if (line.find(']') != std::string::npos){
                initIsDefined = true;
            }

            if(row.size() > 0){
                currentBlock->shape.push_back(row);
            }
            row.clear();

        }
    }

    Block *startingBlock = initial_block;

    while (startingBlock != nullptr){
        rotation(startingBlock);
        startingBlock = startingBlock->next_block;
    }

    removeLastBlock();

    delete startingBlock;


}

void BlockFall::removeLastBlock() {
    if (initial_block == nullptr) {
        // Linked list boşsa bir şey yapma
        return;
    }

    // Eğer sadece bir blok varsa, onu sil ve initial_block'u nullptr yap
    if (initial_block->next_block == nullptr) {
        delete initial_block;
        initial_block = nullptr;
        return;
    }

    // Linked list üzerinde gezerek son bloğu bul
    Block *previousBlock = nullptr;
    Block *currentBlock = initial_block;

    while (currentBlock->next_block != nullptr) {
        previousBlock = currentBlock;
        currentBlock = currentBlock->next_block;
    }

    power_up = currentBlock->shape;

    // Son bloğu sil
    delete currentBlock;

    // Bir önceki bloğun next_block işaretçisini nullptr yap
    if (previousBlock != nullptr) {
        previousBlock->next_block = nullptr;
    }
}


Block *BlockFall::rotateClockwise(Block *&block) {
    int rowNumber = block->shape.size();
    int colNumber = block->shape[0].size();

    Block *rotatedBlock = new Block(); // Yeni bir Block örneği oluştur

    rotatedBlock->shape.resize(colNumber, vector<bool>(rowNumber, false));

    for (int i = 0; i < rowNumber; ++i) {
        for (int j = 0; j < colNumber; ++j) {
            rotatedBlock->shape[j][rowNumber - 1 - i] = block->shape[i][j];
        }
    }

    return rotatedBlock;
}


void BlockFall::initialize_grid(const string &input_file) {

    // Dosyayı aç
    ifstream file(input_file);

    // Dosyanın açılıp açılmadığını kontrol et
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << input_file << endl;
        exit(EXIT_FAILURE);
    }

    // Dosyadan satır satır oku
    string line;
    vector<vector<int>> tempGrid;  // Geçici bir grid

    // Satır ve sütun sayılarını başlat
    rows = 0;
    cols = 0;

    while (getline(file, line)) {
        // Her satırı bir string olarak oku ve stringstream kullanarak parçala
        stringstream ss(line);
        int value;
        vector<int> row;

        while (ss >> value) {
            // Değerleri geçici vektöre ekle
            row.push_back(value);
        }

        // Geçici vektörü ana grid vektörüne ekle
        tempGrid.push_back(row);

        // Satır sayısını güncelle
        rows++;

        // İlk satırdaki eleman sayısını sütun sayısı olarak al
        if (rows == 1) {
            cols = row.size();
        }
    }

    // Dosyayı kapat
    file.close();


    // Ana grid vektörünü başlat
    grid = vector<vector<int>>(rows, vector<int>(cols, 0));

    // Geçici grid değerlerini ana grid'e aktar
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = tempGrid[i][j];
        }
    }

    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    Block *currentBlock = initial_block;
    while (currentBlock != nullptr) {
        Block *nextBlock = currentBlock->next_block;
        delete currentBlock;
        currentBlock = nextBlock;
    }
}



