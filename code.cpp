#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;

class player {
private:
    string name;
    int j_id;
    string type;
    int runs;
    int wickets;
    static int tp; 

public:
    // Constructor
    player(string n = "", int id = 0, string t = "") {
        this->name = n;
        this->j_id = id;
        this->type = t;
        this->runs = 0;
        this->wickets = 0;
        tp++;
    }

    // Destructor
    ~player() {
        tp--;
    }

    void setdata(string name, int j_id, string type) {
        this->name = name;
        this->j_id = j_id;
        this->type = type;
    }

    void setdata(string name, int j_id) {
        this->name = name;
        this->j_id = j_id;
        type = "bowler"; 
    }

    void performance(int runs, int wickets) {
        this->runs = runs;
        this->wickets = wickets;
    }

    int getruns() { return runs; }
    int getwickets() { return wickets; }
    string gettype() { return type; }
    string getname() { return name; }
    int getid() { return j_id; }

    static int getTotalPlayers() {
        return tp; 
    }

    // Comparison operators
    bool operator>(player &p) {  
        if (this->runs > p.runs) return true;
        if (this->runs == p.runs && this->wickets > p.wickets) return true;
        return false;
    }

    bool operator<(player &p) {  
        if (this->runs < p.runs) return true;
        if (this->runs == p.runs && this->wickets < p.wickets) return true;
        return false;
    }

    // Save player details to file
    void saveToFile(ofstream &out) {
        out << name << " "
            << j_id << " "
            << type << " "
            << runs << " "
            << wickets << "\n";
    }

    // Load player details from file
    void loadFromFile(ifstream &in) {
        in >> name >> j_id >> type >> runs >> wickets;
    }

    friend void showplayers(player p);
};

int player::tp = 0;

// Display player in formatted table
void showplayers(player p) {
    cout << left << setw(12) << p.name
         << setw(6) << p.j_id
         << setw(12) << p.type
         << setw(8) << p.runs
         << setw(8) << p.wickets << "\n";
}

// Search player by name
void searchPlayer(vector<player> &players, string name) {
    bool found = false;
    for (auto &pl : players) {
        if (pl.getname() == name) {
            cout << "\nPlayer Found:\n";
            showplayers(pl);
            found = true;
        }
    }
    if (!found) cout << "Player not found!\n";
}

int main() {
    cout << "Enter name, id, type for 5 players consecutively\n";

    int total_runs = 0;
    int total_wickets = 0;

    vector<player> p(5);

    for (int i = 0; i < 5; i++) {
        try {
            string name;
            int j_id;
            string type;

            cout << "\nEnter details for Player " << i + 1 << "\n";
            cout << "Name: ";
            cin >> name;

            while (true) {
                cout << "ID: ";
                cin >> j_id;
                if (cin.fail()) {
                    cout << "Invalid ID\n";
                    cin.clear(); 
                    cin.ignore(10,'\n'); 
                } else {
                    break; 
                }
            }
            
            while (true) {
                cout << "Type (batsman/bowler/allrounder): ";
                cin >> type;
                if (type == "batsman" || type == "bowler" || type == "allrounder") {
                    break;
                } else {
                    cout << "Invalid type! Please enter batsman, bowler, or allrounder only.\n";
                }
            }

            if (type == "bowler") {
                p[i].setdata(name, j_id);
            } else {
                p[i].setdata(name, j_id, type);
            }

            int runs = 0;
            int wickets = 0;
            cout << "Enter runs scored: ";
            cin >> runs;
            string wicket;
            
            while(true){
                cout << "Out or not out (Y/N): ";
                cin >> wicket;
                if (wicket == "Y" || wicket == "y") {
                    wickets = 1;
                    total_wickets++;
                    break;
                } else if(wicket == "N" || wicket == "n") {
                    wickets = 0;
                    break;
                } else {
                    cout << "Wrong input\n";
                }
            }

            p[i].performance(runs, wickets);
            total_runs += runs;

        } catch (exception &e) {
            cout << "Invalid Input\n";
        }
    }

    cout << "\n--- Player List ---\n";
    cout << left << setw(12) << "Name"
         << setw(6) << "ID"
         << setw(12) << "Type"
         << setw(8) << "Runs"
         << setw(8) << "Wickets" << "\n";
    cout << string(50, '-') << "\n";

    for (auto &pl : p) {
        showplayers(pl);
    }

    // Save all players to file
    ofstream fout("players.txt");
    if (fout.is_open()) {
        for (auto &pl : p) {
            pl.saveToFile(fout);
        }
        fout.close();
        cout << "\nAll players saved to players.txt successfully.\n";
    }

    // Find best player
    player best = p[0];
    for (int i = 1; i < 5; i++) {
        if (p[i] > best) {
            best = p[i];
        }
    }

    cout << "\nBest Player:\n";
    showplayers(best);

    cout << "\nTotal players: " << player::getTotalPlayers() << "\n";
    cout << "Total Runs: " << total_runs << "\n";
    cout << "Total Wickets: " << total_wickets << "\n";

    // Sorting players by runs
    sort(p.begin(), p.end());
    cout << "\n--- Players Sorted by Performance ---\n";
    for (auto &pl : p) {
        showplayers(pl);
    }

    // Search
    string searchName;
    cout << "\nEnter player name to search: ";
    cin >> searchName;
    searchPlayer(p, searchName);

    // Load players back from file
    cout << "\n--- Reading from players.txt ---\n";
    ifstream fin("players.txt");
    if (fin.is_open()) {
        vector<player> loadedPlayers;
        while (!fin.eof()) {
            player temp;
            temp.loadFromFile(fin);
            if (!fin.fail()) {
                loadedPlayers.push_back(temp);
            }
        }
        fin.close();

        cout << left << setw(12) << "Name"
             << setw(6) << "ID"
             << setw(12) << "Type"
             << setw(8) << "Runs"
             << setw(8) << "Wickets" << "\n";
        cout << string(50, '-') << "\n";
        for (auto &pl : loadedPlayers) {
            showplayers(pl);
        }
    }

    return 0;
}
