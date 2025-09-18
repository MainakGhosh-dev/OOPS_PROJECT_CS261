#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <exception>
using namespace std;
class Player {
public:
    string name;
    int r, b, four, six, w, ov, con;

    Player(string n = "") {
        this->name = n;
        this->r = this->b = this->four = this->six = this->w = this->ov = this->con = 0;
    }
    
    inline int strikeRate()  {
        if (this->b == 0) {
            return 0;
        }
        return (this->r * 100.0) / this->b;
    }
    
    inline int economy() {
        if (this->ov == 0) {
            return 0;
        }
        return this->con / (int)this->ov;
    }

    Player& operator++() {
        this->b++;
        return *this;
    }

    Player operator+(const Player& o) {
        Player res("Combined");
        res.r = this->r + o.r;
        res.b = this->b + o.b;
        res.four = this->four + o.four;
        res.six = this->six + o.six;
        res.w = this->w + o.w;
        res.ov = this->ov + o.ov;
        res.con = this->con + o.con;
        return res;
    }
    void addBall() {
         this->b++; 
        }
    void addConceded(int runs) {
         this->con += runs; 
        }
};

class Team {
public:
    string name;
    Player* pl;
    int cnt;
    int tr, wkt, ob, bb;
    Team(string n = "", int pCnt = 0) {
        this->name = n;
        this->cnt = pCnt;
        this->tr = this->wkt = this->ob = this->bb = 0;
        if (pCnt > 0) {
            try {
                this->pl = new Player[pCnt];
            } catch (bad_alloc& e) {
                cout << " error: "<<endl;
                this->pl = nullptr;
                this->cnt = 0;
            }
        } else {
            this->pl = nullptr;
        }
    }
    void addPlayer(string& pname, int idx) {
        this->pl[idx] = Player(pname);
    }
    void addPlayer( string& pname, int idx, int runs, int balls, int fours, int sixes) {
        this->pl[idx] = Player(pname);
        this->pl[idx].r = runs;
        this->pl[idx].b = balls;
        this->pl[idx].four = fours;
        this->pl[idx].six = sixes;
    }

    void showb() {
        cout << "\nBatting " << this->name << "\n";
        cout << "Name\tRuns\tBalls\t4s\t6s\tSR\n";
        for (int i = 0; i < this->cnt; ++i) {
            cout << this->pl[i].name << "\t" << this->pl[i].r << "\t" << this->pl[i].b << "\t" << this->pl[i].four << "\t" << this->pl[i].six << "\t" << this->pl[i].strikeRate() << "\n";
        }
    }

    void showbo() {
        cout << "\nBowling " << this->name << "\n";
        cout << "Name\tOvers\tRuns\tWkts\tEco\n";
        for (int i = 0; i < this->cnt; ++i) {
            if (this->pl[i].ov > 0) {
                cout << this->pl[i].name << "\t" << this->pl[i].ov << "\t"<< this->pl[i].con << "\t"<< this->pl[i].w << "\t" << this->pl[i].economy() << "\n";
            }
        }
    }

    ~Team() {
        delete[] pl;
    }
};

class Match {
public:
    Team tA, tB;
    int ov;
    int str, ns, bow;

    Match(string a, string b, int o, int pCnt) {
        tA = Team(a, pCnt);
        tB = Team(b, pCnt);
        ov = o;
        str = ns = bow = -1;
    }

    void sti(Team &bat, Team &bowl, bool is2nd = false, int tgt = 0, int n = 0);
    void upsc(Team &bat, Team &bowl, int evt);
    bool checkEnd(Team &bat, Team &bowl, int tgt, int n);
};

void Match::sti(Team &bat, Team &bowl, bool is2nd, int tgt, int n) {
    cout << "\nStarting innings: " << bat.name << " batting\n";
    bat.tr = bat.wkt = bat.ob = bat.bb = 0;
    
    do {
        cout << "Enter striker index (0-" << bat.cnt - 1 << "): ";
        cin >> this->str;
        if (cin.fail() || this->str < 0 || this->str >= bat.cnt) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid index\n";
            this->str = -1;
        }
    } while (this->str == -1);
    
    do {
        cout << "Enter non-striker index: ";
        cin >> this->ns;
        if (cin.fail()) {
            cin.clear(); cin.ignore(100, '\n');
            cout << "invalid \n";
            this->ns = -1;
        }
    } while (this->ns == -1);

    do {
        cout << "Enter bowler index from " << bowl.name << " (0-" << bowl.cnt - 1 << "): ";
        cin >> this->bow;
        if (cin.fail() || this->bow < 0 || this->bow >= bowl.cnt) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "Invalid index\n";
            this->bow = -1;
        }
    } while (this->bow == -1);

    while (bat.ob < this->ov && bat.wkt < bat.cnt - 1) {
        int evt;
        cout << "\nBall " << bat.ob << "." << (bat.bb % 6) + 1;
        cout << " - Enter event (0-6 runs, 7=wide, 9=wicket, 10=bye, 11=leg-bye) ";
        
        if (!(cin >> evt)) {
            cin.clear(); cin.ignore(10, '\n');
            cout << "Invalid input\n";
            continue;
        }
        if (evt > 12) break;
        this->upsc(bat, bowl, evt);

        if (is2nd && this->checkEnd(bat, bowl, tgt, n)) {
            return;
        }
        if (bat.ob >= this->ov || bat.wkt >= bat.cnt - 1) {
            break;
        }

        if (bat.bb % 6 == 0 && bat.bb > 0) {
            int opt = 0;
            do {
                cout << "\n Over  \n1. Show Batting\n2. Show Bowling\n3. Continue\nChoice: ";
                if (!(cin >> opt)) {
                    cin.clear(); cin.ignore(10, '\n');
                    cout << "invalid\n";
                    continue;
                }
                if (opt == 1) {
                    bat.showb();
                }
                else if (opt == 2) {
                    bowl.showbo();
                }
            } while (opt != 3);
            if (bat.ob < ov) {
                do {
                    cout << "\nNew bowler index: ";
                    cin >> this->bow;
                    if (cin.fail()) {
                        cin.clear(); cin.ignore(10000, '\n');
                        cout << "Invalid index\n";
                        this->bow = -2;
                    }
                } while (this->bow == -2);
            }
        }
    }
}

void Match::upsc(Team &bat, Team &bowl, int evt) {
    try {
        Player &batter = bat.pl[this->str];
        Player &bowler = bowl.pl[this->bow];
        int runs = 0;

        if (evt >= 0 && evt <= 6) {
            batter.r += evt;
            ++batter;
            bat.tr += evt;
            bowler.addConceded(evt);
            if (evt == 4) batter.four++;
            if (evt == 6) batter.six++;
            runs = evt;
        } else if (evt == 7) {
            bat.tr += 1;
            bowler.addConceded(1);
        } else if (evt == 9) {
            bat.wkt++;
            ++batter;
            bowler.w++;
            if (bat.wkt < bat.cnt - 1) {
                do {
                    cout << "New batsman index: ";
                    cin >> this->str;
                    if (cin.fail() || this->str < 0 || this->str >= bat.cnt) {
                        cin.clear(); cin.ignore(10000, '\n');
                        cout << "Invalid index\n";
                        this->str = -1;
                    }
                } while (this->str == -1);
            }
        } else if (evt == 10 || evt == 11) {
            cout << "Enter runs: ";
            cin >> runs;
            if (!cin) {
                cin.clear();
                cin.ignore(10, '\n');
                runs = 0;
                cout << "Invalid\n";
            }
            bat.tr += runs;
            bowler.addConceded(runs);
        }

        if (evt != 7) {
            bat.bb++;
            if (bat.bb % 6 == 0) {
                bat.ob++;
                bowler.ov++;
                swap(this->str, this->ns);
            } else if (runs % 2 != 0) {
                swap(this->str, this->ns);
            }
        }
    } catch (exception& e) {
        cout << "Error" << endl;
    }
}

bool Match::checkEnd(Team &bat, Team &bowl, int tgt, int n) {
    if (bat.tr > tgt) {
        cout << "\nResult\n" << bat.name << " won by " << (n - bat.wkt - 1) << " wickets\n";
        return true;
    }
    if (bat.wkt >= n - 1 && bat.tr <= tgt) {
        cout << "\nResult\n" << bowl.name << " won by " << (tgt - bat.tr) << " runs\n";
        return true;
    }
    return false;
}

int main() {
    try {
        string t1, t2;
        int ov, n, fb;
        cout << "Team A name: ";
        getline(cin, t1);
        cout << "Team B name: ";
        getline(cin, t2);
        cout << "Total overs: ";
        cin >> ov;
        if (!cin || ov <= 0) {
            cout << "Invalid\n";
            ov = 1;
            cin.clear();
            cin.ignore(10, '\n');
        }

        cout << "Players per team: ";
        cin >> n;
        if (n <= 1) {
            cout << "Invalid\n";
            n = 2;
        }
        cin.ignore();

        Match match(t1, t2, ov, n);

        cout << "\nTeam A players:\n";
        for (int i = 0; i < n; i++) {
            string name;
            cout << "Player " << i << ": ";
            getline(cin, name);
            match.tA.addPlayer(name, i);
        }

        cout << "\nTeam B players:\n";
        for (int i = 0; i < n; i++) {
            string name;
            cout << "Player " << i << ": ";
            getline(cin, name);
            match.tB.addPlayer(name, i);
        }

        cout << "\nBat first? (1=" << t1 << ", 2=" << t2 << "): ";
        cin >> fb;
        if (fb != 1 && fb != 2) {
            cout << "Invalid. Setting to 1.\n";
            fb = 1;
        }
        
        if (fb == 1) {
            match.sti(match.tA, match.tB);
            match.tA.showb();
            match.tB.showbo();
            match.sti(match.tB, match.tA, true, match.tA.tr, n);
            match.tB.showb();
            match.tA.showbo();
        } else {
            match.sti(match.tB, match.tA);
            match.tB.showb();
            match.tA.showbo();
            match.sti(match.tA, match.tB, true, match.tB.tr, n);
            match.tA.showb();
            match.tB.showbo();
        }
        
        cout << "\nFinal Result \n";
        if (match.tA.tr > match.tB.tr) {
            cout << "Team A won\n";
        } else if (match.tB.tr > match.tA.tr) {
            cout << "Team B won\n";
        } else {
            cout << "Draw\n";
        }
    } catch (const exception& e) {
        cout << "Error occurred\n";
        return 1;
    }
    return 0;
}
