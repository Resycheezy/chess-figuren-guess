#include <cctype> // fuer toupper()
#include <iostream>
#include <string>
using namespace std;

// Klasse fuer eine einzelne Schachfigur (Singular, da ein Objekt = eine Figur)
class Figur {
public:
  string name;
  string farbe;
  string position;

  // Konstruktor mit Initialisierungsliste (effizienter als Zuweisung im Rumpf)
  Figur(string n, string f, string p) : name(n), farbe(f), position(p) {}

  // Statische Daten: gehoeren zur Klasse, nicht zu einzelnen Objekten.
  // So wird der Speicher nur einmal belegt, egal wie viele Figuren erstellt
  // werden.
  static const string figuren_positionen[32];
  static const string start_namen[32];
  static const string farben[2];
  static const string namen[6];

  // Alle Startpositionen ausgeben
  static void figuren_anzeigen() {
    for (int i = 0; i < 32; i++) {
      cout << figuren_positionen[i] << " ";
    }
    cout << endl;
  }

  // Gibt den Namen der Figur zurueck, die auf der gegebenen Position steht.
  // Liefert einen leeren String, wenn keine Figur dort steht.
  static string figur_auf_position(const string &pos) {
    for (int i = 0; i < 32; i++) {
      if (figuren_positionen[i] == pos) {
        return start_namen[i];
      }
    }
    return "";
  }
};

// Definition der statischen Member ausserhalb der Klasse
const string Figur::figuren_positionen[32] = {
    "A1", "B1", "C1", "D1", "E1", "F1", "G1", "H1", // Weiss Offiziere
    "A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2", // Weiss Bauern
    "A7", "B7", "C7", "D7", "E7", "F7", "G7", "H7", // Schwarz Bauern
    "A8", "B8", "C8", "D8", "E8", "F8", "G8", "H8"  // Schwarz Offiziere
};
const string Figur::start_namen[32] = {
    "weisser Turm",       "weisser Springer",   "weisser Laeufer",
    "weisse Dame",        "weisser Koenig",     "weisser Laeufer",
    "weisser Springer",   "weisser Turm",       "weisser Bauer",
    "weisser Bauer",      "weisser Bauer",      "weisser Bauer",
    "weisser Bauer",      "weisser Bauer",      "weisser Bauer",
    "weisser Bauer",      "schwarzer Bauer",    "schwarzer Bauer",
    "schwarzer Bauer",    "schwarzer Bauer",    "schwarzer Bauer",
    "schwarzer Bauer",    "schwarzer Bauer",    "schwarzer Bauer",
    "schwarzer Turm",     "schwarzer Springer", "schwarzer Laeufer",
    "schwarze Dame",      "schwarzer Koenig",   "schwarzer Laeufer",
    "schwarzer Springer", "schwarzer Turm"};
const string Figur::farben[2] = {"weiss", "schwarz"};
const string Figur::namen[6] = {"Bauer",   "Turm", "Springer",
                                "Laeufer", "Dame", "Koenig"};

// Klasse fuer das Schachbrett (Grossbuchstabe als Konvention)
class Schachbrett {
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
        } else {
          cout << "# ";
        }
      }
      cout << endl;
    }
    cout << "  A B C D E F G H" << endl;
  }
};

int main() {

  string name = "bob:";
  char spalte; // char statt string, da es nur ein einzelnes Zeichen ist
  char reihe;  // char statt string
  string guess;

  Schachbrett brett;

  cout << name << endl;

  cout << "\nKoordinaten:\n";
  brett.schachbrett_buchstaben();

  cout << "\nSchachbrett:\n";
  brett.zeichnen();

  while (true) {
    cout
        << name
        << " Bitte gib eine Koordinate ein (z.B. A1) oder 'exit' zum Beenden: ";
    cin >> guess;

    if (guess == "exit") {
      break;
    } else if (guess.length() == 2) {
      // toupper() sorgt dafuer, dass auch Kleinbuchstaben akzeptiert werden
      spalte = toupper(guess[0]);
      reihe = guess[1];

      // Pruefen, ob die Koordinate tatsaechlich auf dem Brett liegt
      if (spalte >= 'A' && spalte <= 'H' && reihe >= '1' && reihe <= '8') {
        cout << "Du hast die Koordinaten " << spalte << reihe << " eingegeben."
             << endl;

        // Figur auf diesem Feld nachschlagen (nutzt jetzt die Figur-Klasse!)
        string pos = string(1, spalte) + string(1, reihe);
        string figur = Figur::figur_auf_position(pos);
        if (!figur.empty()) {
          cout << "Auf " << pos << " steht: " << figur << endl;
        } else {
          cout << "Auf " << pos << " steht keine Figur." << endl;
        }
      } else {
        cout << "Ungueltige Koordinate! Spalte muss A-H und Reihe 1-8 sein."
             << endl;
      }
    } else {
      cout << "Ungueltige Eingabe. Bitte gib eine gueltige Koordinate ein "
              "(z.B. A1) oder 'exit' zum Beenden."
           << endl;
    }
  }
  return 0;
}
