#include "Leaderboard.h"
#include "fstream"
#include "iostream"
void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).

    // Başlangıçta liderlik tablosu boşsa veya yeni girişin puanı en yüksekse
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        // Yeni girişi başa ekle
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry *current = head_leaderboard_entry;
        LeaderboardEntry *prev = nullptr;

        // Puan sırasına göre doğru konumu bul
        while (current && new_entry->score <= current->score) {
            prev = current;
            current = current->next_leaderboard_entry;
        }

        // Yeni girişi ekleyin
        prev->next_leaderboard_entry = new_entry;
        new_entry->next_leaderboard_entry = current;

        // Liderlik tablosunun boyutunu kontrol et
        int count = 0;
        current = head_leaderboard_entry;
        LeaderboardEntry *prev_entry = nullptr;
        while (current && count < 10) {
            prev_entry = current;
            current = current->next_leaderboard_entry;
            count++;
        }

        // Liderlik tablosu 10 girişi geçiyorsa en düşük puanlı girişi kaldır
        if (count == 10 && prev_entry) {
            prev_entry->next_leaderboard_entry = nullptr;
            delete current;
        }
    }

}




void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions

    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Dosya açma hatası!" << endl;
        return;
    }

    LeaderboardEntry* current_entry = head_leaderboard_entry;

    // Her girişi dosyaya yaz
    while (current_entry != nullptr) {
        file << "Score: " << current_entry->score
             << ", Last Played: " << current_entry->last_played
             << ", Player Name: " << current_entry->player_name << endl;

        current_entry = current_entry->next_leaderboard_entry;
    }

    file.close();
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
}
