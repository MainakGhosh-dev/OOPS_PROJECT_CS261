void Match::sti(Team &bat, Team &bowl, bool is2nd, int tgt, int n) {
    cout << "\nstarting  " << bat.name << "batting\n";
    bat.tr = bat.wkt = bat.ob = bat.bb = 0;

    this->str = -1;
    while (this->str == -1) {
        cout << "Enter striker index (0 to " << bat.cnt - 1 << "): ";
        cin >> this->str;
        if (cin.fail() || this->str < 0 || this->str >= bat.cnt) {
            cin.clear(); cin.ignore(10, '\n');
            cout << "Invalid\n";
            this->str = -1;
        }
    }

    this->ns = -1;
    while (this->ns == -1) {
        cout << "enter non-striker index: ";
        cin >> this->ns;
        if (cin.fail()) {
            cin.clear(); cin.ignore(10, '\n');
            cout << "Invalid\n";
            this->ns = -1;
        }
    }

    this->bow = -1;
    while (this->bow == -1) {
        cout << "Enter bowler index from " << bowl.name << " (0 to " << bowl.cnt - 1 << "): ";
        cin >> this->bow;
        if (cin.fail() || this->bow < 0 || this->bow >= bowl.cnt) {
            cin.clear(); cin.ignore(10, '\n');
            cout << "Invalid index\n";
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
            }

            if (bat.ob < ov) {
                this->bow = -2;
                while (this->bow == -2) {
                    cout << "\nnew bowler index: ";
                    cin >> this->bow;
                    if (cin.fail()) {
                        cin.clear(); cin.ignore(10, '\n');
                        cout << "invalid \n";
                        this->bow = -2;
                    }
                }
            }
        }
    }
}