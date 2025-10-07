#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <exception>
using namespace std;

class BaseStats {
public:
    virtual void showStats() {
        cout << "Cricket game.\n";
    }
    virtual void announceWinner(const string& teamName) {
        cout << "Winner: " << teamName << endl;
    }
    virtual ~BaseStats() {}
};

class Batting : virtual public BaseStats {
public:
    virtual void battingSkill() {
        cout << "Displays batting performance summary.\n";
    }
};

class Bowling : virtual public BaseStats {
public:
    virtual void bowlingSkill() {
        cout << "Displays bowling performance summary.\n";
    }
};

class Player : public Batting, public Bowling {
public:
    string name;
    int r, b, four, six, w, ov, con;

    Player(string n = "") {
        this->name = n;
        this->r = this->b = this->four = this->six = this->w = this->ov = this->con = 0;
    }

    virtual int strikeRate() {
        if (this->b == 0) return 0;
        return (this->r * 100.0) / this->b;
    }

    virtual int economy() {
        if (this->ov == 0) return 0;
        return this->con / (int)this->ov;
    }

    Player& operator++() {
        this->b++;
        return *this;
    }

    void addBall() { this->b++; }
    void addConceded(int runs) { this->con += runs; }

    virtual void showStats() {
        cout << "Player Stats: " << name << "  Runs: " << r << "  Balls: " << b << endl;
    }

    virtual ~Player() {}
};

class Team : public Player {
public:
    Player* pl;
    int cnt;
    int tr, wkt, ob, bb;

    Team(string n = "", int pCnt = 0) : Player(n) {
        this->cnt = pCnt;
        this->tr = this->wkt = this->ob = this->bb = 0;
        if (pCnt > 0) {
            try {
                this->pl = new Player[pCnt];
            } catch (bad_alloc& e) {
                cout << " error: " << endl;
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

    void showb() {
        cout << "\nBatting " << this->name << "\n";
        cout << "Name  Runs  Balls  4s  6s  SR\n";
        for (int i = 0; i < this->cnt; ++i) {
            cout << this->pl[i].name << "  " << this->pl[i].r << "  " << this->pl[i].b << "  " 
                 << this->pl[i].four << "  " << this->pl[i].six << "  " << this->pl[i].strikeRate() << "\n";
        }
    }

    void showbo() {
        cout << "\nBowling " << this->name << "\n";
        cout << "Name  Overs  Runs  Wkts  Eco\n";
        for (int i = 0; i < this->cnt; ++i) {
            if (this->pl[i].ov > 0) {
                cout << this->pl[i].name << "  " << this->pl[i].ov << "  " << this->pl[i].con 
                     << "  " << this->pl[i].w << "  " << this->pl[i].economy() << "\n";
            }
        }
    }

    virtual void showStats() {
        cout << "Team Stats: " << name << "  Total Runs: " << tr << "  Wickets: " << wkt << endl;
    }

    virtual void announceWinner(const string& teamName) {
        cout << "🏆 Team " << teamName << " wins the match!\n";
    }

    ~Team() {
        delete[] pl;
    }
};

class Match : public Team {
public:
    Team tA, tB;
    int ov;
    int str, ns, bow;

    Match(string a, string b, int o, int pCnt)
        : tA(a, pCnt), tB(b, pCnt) {
        ov = o;
        str = ns = bow = -1;
    }

    void sti(Team &bat, Team &bowl, bool is2nd = false, int tgt = 0, int n = 0);
    void upsc(Team &bat, Team &bowl, int evt);
    bool checkEnd(Team &bat, Team &bowl, int tgt, int n);
};

void Match::sti(Team &bat, Team &bowl, bool is2nd, int tgt, int n) {
    cout << "\nstarting  " << bat.name << " batting\n";
    bat.tr = bat.wkt = bat.ob = bat.bb = 0;

    this->str = -1;
    while (this->str == -1) {
        cout << "Enter striker index (0 to " << bat.cnt - 1 << "): ";
        cin >> this->str;
        if (cin.fail() || this->str < 0 || this->str >= bat.cnt) {
            cin.clear();
            cin.ignore(10, '\n');
            cout << "Invalid\n";
            this->str = -1;
        }
    }

    this->ns = -1;
    while (this->ns == -1) {
        cout << "enter non-striker index: ";
        cin >> this->ns;
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(10, '\n');
            cout << "Invalid\n";
            this->ns = -1;
        }
    }

    this->bow = -1;
    while (this->bow == -1) {
        cout << "Enter bowler index from " << bowl.name << " (0 to " << bowl.cnt - 1 << "): ";
        cin >> this->bow;
        if (cin.fail() || this->bow < 0 || this->bow >= bowl.cnt) {
            cin.clear(); 
            cin.ignore(10, '\n');
            cout << "Invalid \n";
            this->bow = -1;
        }
    }

    while (bat.ob < this->ov && bat.wkt < bat.cnt - 1) {
        int evt;
        cout << "\nBall " << bat.ob << "." << (bat.bb % 6) + 1;
        cout << " - Enter event (0-6 runs, 7=wide, 9=wicket, 10=bye, 11=leg-bye): ";

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
            while (opt != 3) {
                cout << "\n\n1. Show Batting\n2. Show Bowling\n3. Continue\nChoice: ";
                if (!(cin >> opt)) {
                    cin.clear(); 
                    cin.ignore(10, '\n');
                    cout << "invalid\n";
                    continue;
                }
                if (opt == 1) {
                    bat.showb();
                }
                else if (opt == 2) {
                    bowl.showbo();
                }
            }

            if (bat.ob < ov) {
                this->bow = -2;
                while (this->bow == -2) {
                    cout << "\nnew bowler index: ";
                    cin >> this->bow;
                    if (cin.fail()) {
                        cin.clear(); 
                        cin.ignore(10, '\n');
                        cout << "invalid \n";
                        this->bow = -2;
                    }
                }
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
                    cout << "new batsman index: ";
                    cin >> this->str;
                    if (cin.fail() || this->str < 0 || this->str >= bat.cnt) {
                        cin.clear(); cin.ignore(10, '\n');
                        cout << "invalid index\n";
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
                cout << "invalid\n";
            }
            bat.tr += runs;
            bowler.addConceded(runs);
        }

        if (evt != 7) {
            bat.bb++;
            ++batter;
            bowler.addBall(); 
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

        cout << "\nbat first (1=" << t1 << ", 2=" << t2 << "):";
        cin >> fb;
        if (fb != 1 && fb != 2) {
            cout << "invalid.\n";
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
        BaseStats* bs;
        if (match.tA.tr > match.tB.tr) {
            bs = &match.tA;
            bs->announceWinner(match.tA.name);
        } else if (match.tB.tr > match.tA.tr) {
            bs = &match.tB;
            bs->announceWinner(match.tB.name);
        } else {
            cout << "Draw\n";
        }

        cout << "\nPost-Match Summary \n";
        BaseStats* summary;

        summary = &match.tA;
        summary->showStats();

        summary = &match.tB;
        summary->showStats();

        int sampleIndex = 0;
        summary = &match.tA.pl[sampleIndex];
        summary->showStats();

    } catch (const exception& e) {
        cout << "Error occurred\n";
        return 1;
    }
    return 0;
}
