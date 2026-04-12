#include <iostream>
#include <string>
using namespace std;

class figuren {
public:
    string name;
    string farbe;
    string position;
    figuren(string n, string f, string p) {
        name = n;
        farbe = f;
        position = p;
    }
    string figurgen_positions[32] = {
        "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1", // Weiß Offiziere
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2", // Weiß Bauern
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7", // Schwarz Bauern
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"  // Schwarz Offiziere
    };
    string start_namen[32] = {
    "weisser Turm", "weisser Springer", "weisser Laeufer", "weisse Dame", "weisser Koenig", "weisser Laeufer", "weisser Springer", "weisser Turm",
    "weisser Bauer", "weisser Bauer", "weisser Bauer", "weisser Bauer", "weisser Bauer", "weisser Bauer", "weisser Bauer", "weisser Bauer",
    "schwarzer Bauer", "schwarzer Bauer", "schwarzer Bauer", "schwarzer Bauer", "schwarzer Bauer", "schwarzer Bauer", "schwarzer Bauer", "schwarzer Bauer",
    "schwarzer Turm", "schwarzer Springer", "schwarzer Laeufer", "schwarze Dame", "schwarzer Koenig", "schwarzer Laeufer", "schwarzer Springer", "schwarzer Turm"
    };
    string farben[2] = { "weiss", "schwarz" };
    string namen[6] = { "Bauer", "Turm", "Springer", "Laeufer", "Dame", "Koenig" };
    void figuren_anzeigen() {
        for (int i = 0; i < 32; i++) {
            cout << figurgen_positions[i] << " ";
        }
    
    };
};
class schachbrett {
public:
    void schachbrett_buchstaben() {
        for (char reihe = '1'; reihe <= '8'; reihe++) {
            for (char spalte = 'A'; spalte <= 'H'; spalte++) {
                cout << spalte << reihe << " ";
            }
            cout << endl;
        }
    }

    void zeichnen() {
        for (int reihe = 8; reihe >= 1; reihe--) {
            cout << reihe << " ";
            for (int spalte = 0; spalte < 8; spalte++) {
                if ((reihe + spalte) % 2 == 0) {
                    cout << ". ";
                }
                else {
                    cout << "# ";
                }
            }
            cout << endl;
        }
        cout << "  A B C D E F G H" << endl;
    }
};

int main(){

    string name = "bob:";
    string spalte;
    string reihe;
    string guess;

    schachbrett brett;

    cout << name << endl;

    cout << "\nKoordinaten:\n";
    brett.schachbrett_buchstaben();

    cout << "\nSchachbrett:\n";
    brett.zeichnen();

    while (true) {
		cout << name << " Bitte gib eine Koordinate ein (z.B. A1) oder 'exit' zum Beenden: ";
        cin >> guess;
        if (guess == "exit") {
            break;
        }
        else if (guess.length() == 2) {
            spalte = guess[0];
            reihe = guess[1];
            cout << "Du hast die Koordinaten " << spalte << reihe << " eingegeben." << endl;
        }
        else {
            cout << "Ungueltige Eingabe. Bitte gib eine gueltige Koordinate ein (z.B. A1) oder 'exit' zum Beenden." << endl;
        }
    }
    return 0;
}
