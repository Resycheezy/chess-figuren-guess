#include <cctype>  // fuer toupper(), tolower(), isspace()
#include <iostream>
#include <random>  // fuer Zufallszahlen im Quiz
#include <string>
#include <vector>
using namespace std;

// ---------------------------------------------------------------------------
// Eine einzelne Schachfigur
// ---------------------------------------------------------------------------
class Figur {
public:
  string name;     // z.B. "Turm"
  string farbe;    // "weiss" oder "schwarz"
  string position; // z.B. "A1"

  Figur(const string &n, const string &f, const string &p)
      : name(n), farbe(f), position(p) {}

  // "weisser Turm", "schwarze Dame", ...
  string beschreibung() const {
    string endung = (name == "Dame") ? "e " : "er ";
    return farbe + endung + name;
  }

  // Ein Buchstabe fuers Brett: K D T L S B (deutsche Kuerzel).
  // Praktischerweise ist das einfach der erste Buchstabe des Namens.
  // Weiss = Grossbuchstabe, Schwarz = Kleinbuchstabe.
  char symbol() const {
    char s = name[0];
    return (farbe == "weiss") ? s : static_cast<char>(tolower(s));
  }
};

// ---------------------------------------------------------------------------
// Das Schachbrett mit allen Figuren in der Startaufstellung
// ---------------------------------------------------------------------------
class Schachbrett {
public:
  Schachbrett() {
    const string offiziere[8] = {"Turm", "Springer", "Laeufer", "Dame",
                                 "Koenig", "Laeufer", "Springer", "Turm"};
    // Statt 32 Positionen von Hand zu tippen, bauen wir sie in einer Schleife.
    for (int i = 0; i < 8; i++) {
      string spalte(1, static_cast<char>('A' + i));
      figuren.emplace_back(offiziere[i], "weiss", spalte + "1");
      figuren.emplace_back("Bauer", "weiss", spalte + "2");
      figuren.emplace_back("Bauer", "schwarz", spalte + "7");
      figuren.emplace_back(offiziere[i], "schwarz", spalte + "8");
    }
  }

  // Liefert die Figur auf dem Feld oder nullptr, wenn das Feld leer ist.
  const Figur *figur_auf(const string &pos) const {
    for (const Figur &f : figuren) {
      if (f.position == pos) {
        return &f;
      }
    }
    return nullptr;
  }

  const vector<Figur> &alle_figuren() const { return figuren; }

  // Zeichnet das Brett. Leere Felder: '.' = helles Feld, '#' = dunkles Feld.
  void zeichnen(bool mit_figuren) const {
    cout << endl;
    for (int reihe = 8; reihe >= 1; reihe--) {
      cout << reihe << "  ";
      for (int spalte = 0; spalte < 8; spalte++) {
        string pos = string(1, static_cast<char>('A' + spalte)) +
                     static_cast<char>('0' + reihe);
        const Figur *f = figur_auf(pos);
        if (mit_figuren && f != nullptr) {
          cout << f->symbol() << ' ';
        } else if ((reihe + spalte) % 2 == 0) {
          cout << ". ";
        } else {
          cout << "# ";
        }
      }
      cout << endl;
    }
    cout << "   A B C D E F G H" << endl << endl;
  }

private:
  vector<Figur> figuren;
};

// ---------------------------------------------------------------------------
// Hilfsfunktionen fuer die Eingabe
// ---------------------------------------------------------------------------

// Liest eine Zeile, entfernt Leerzeichen am Rand.
// Gibt false zurueck, wenn keine Eingabe mehr kommt (z.B. Ctrl+Z / Ctrl+D) -
// sonst wuerde das Programm in einer Endlosschleife haengen.
bool zeile_lesen(string &eingabe) {
  if (!getline(cin, eingabe)) {
    return false;
  }
  size_t anfang = 0;
  while (anfang < eingabe.size() && isspace((unsigned char)eingabe[anfang]))
    anfang++;
  size_t ende = eingabe.size();
  while (ende > anfang && isspace((unsigned char)eingabe[ende - 1]))
    ende--;
  eingabe = eingabe.substr(anfang, ende - anfang);
  return true;
}

string klein(string s) {
  for (char &c : s) {
    c = static_cast<char>(tolower((unsigned char)c));
  }
  return s;
}

// Macht aus "e4", " E4 " usw. ein sauberes "E4".
// Gibt "" zurueck, wenn es keine gueltige Koordinate ist.
string koordinate_pruefen(const string &eingabe) {
  if (eingabe.length() != 2) {
    return "";
  }
  char spalte = static_cast<char>(toupper((unsigned char)eingabe[0]));
  char reihe = eingabe[1];
  if (spalte < 'A' || spalte > 'H' || reihe < '1' || reihe > '8') {
    return "";
  }
  return string(1, spalte) + reihe;
}

// Prueft, ob die Eingabe den Figurennamen trifft.
// Akzeptiert: "Turm", "turm", "T", "weisser Turm", "läufer"/"laufer" usw.
bool name_passt(const string &eingabe, const Figur &f) {
  string e = klein(eingabe);
  string n = klein(f.name);

  if (e == klein(f.beschreibung()) || e == n) {
    return true;
  }
  // Nur der Buchstabe, z.B. "t" fuer Turm
  if (e.length() == 1 && e[0] == n[0]) {
    return true;
  }
  // Ohne "e" nach Umlaut: "laufer", "konig"
  if ((n == "laeufer" && e == "laufer") || (n == "koenig" && e == "konig")) {
    return true;
  }
  // Mit echtem Umlaut. Je nach Konsole kommt das als UTF-8 (2 Bytes) oder
  // als einzelnes Windows-Zeichen an - wir suchen einfach nach "ufer"/"nig".
  if (n == "laeufer" && e.size() >= 5 && e[0] == 'l' &&
      e.substr(e.size() - 4) == "ufer") {
    return true;
  }
  if (n == "koenig" && e.size() >= 4 && e[0] == 'k' &&
      e.substr(e.size() - 3) == "nig") {
    return true;
  }
  return false;
}

// ---------------------------------------------------------------------------
// Spielmodi
// ---------------------------------------------------------------------------

// Modus 1: Koordinate eingeben -> Figur wird angezeigt (zum Lernen)
void erkunden(const Schachbrett &brett) {
  brett.zeichnen(true);
  cout << "Grossbuchstaben = Weiss, Kleinbuchstaben = Schwarz\n"
       << "K = Koenig, D = Dame, T = Turm, L = Laeufer, S = Springer, "
          "B = Bauer\n\n";

  string eingabe;
  while (true) {
    cout << "Koordinate (z.B. E1) oder 'm' fuer Menue: ";
    if (!zeile_lesen(eingabe) || klein(eingabe) == "m") {
      return;
    }
    string pos = koordinate_pruefen(eingabe);
    if (pos.empty()) {
      cout << "Ungueltig! Spalte A-H, Reihe 1-8, z.B. E1.\n";
      continue;
    }
    const Figur *f = brett.figur_auf(pos);
    if (f != nullptr) {
      cout << "Auf " << pos << " steht: " << f->beschreibung() << "\n";
    } else {
      cout << "Auf " << pos << " steht keine Figur.\n";
    }
  }
}

// Modus 2: "Welche Figur steht auf E1?"
void quiz_welche_figur(const Schachbrett &brett, mt19937 &zufall) {
  const vector<Figur> &figuren = brett.alle_figuren();
  uniform_int_distribution<size_t> wuerfel(0, figuren.size() - 1);
  const int runden = 10;
  int punkte = 0;
  int beantwortet = 0;

  cout << "\nWelche Figur steht in der Startaufstellung auf dem Feld?\n"
       << "Antworte z.B. mit 'Turm' oder 'T'. 'm' bricht ab.\n";
  brett.zeichnen(false);

  for (int runde = 1; runde <= runden; runde++) {
    const Figur &gesucht = figuren[wuerfel(zufall)];
    cout << "[" << runde << "/" << runden << "] Was steht auf "
         << gesucht.position << "? ";

    string eingabe;
    if (!zeile_lesen(eingabe) || klein(eingabe) == "m") {
      break;
    }
    beantwortet++;
    if (name_passt(eingabe, gesucht)) {
      cout << "  Richtig! (" << gesucht.beschreibung() << ")\n";
      punkte++;
    } else {
      cout << "  Leider falsch - dort steht: " << gesucht.beschreibung()
           << ".\n";
    }
  }
  cout << "\nErgebnis: " << punkte << " von " << beantwortet << " Punkten.\n";
}

// Modus 3: "Wo steht der weisse Koenig?"
void quiz_wo_steht(const Schachbrett &brett, mt19937 &zufall) {
  const vector<Figur> &figuren = brett.alle_figuren();
  uniform_int_distribution<size_t> wuerfel(0, figuren.size() - 1);
  const int runden = 10;
  int punkte = 0;
  int beantwortet = 0;

  cout << "\nAuf welchem Feld steht die Figur zu Spielbeginn?\n"
       << "Bei Figuren, die es mehrfach gibt, zaehlt jedes passende Feld. "
          "'m' bricht ab.\n";
  brett.zeichnen(false);

  for (int runde = 1; runde <= runden; runde++) {
    const Figur &gesucht = figuren[wuerfel(zufall)];
    cout << "[" << runde << "/" << runden << "] Gesucht: "
         << gesucht.beschreibung() << " - welches Feld? ";

    string eingabe;
    if (!zeile_lesen(eingabe) || klein(eingabe) == "m") {
      break;
    }
    beantwortet++;
    string pos = koordinate_pruefen(eingabe);
    const Figur *dort = pos.empty() ? nullptr : brett.figur_auf(pos);

    // Richtig, wenn dort eine Figur mit gleichem Namen UND gleicher Farbe steht
    if (dort != nullptr && dort->name == gesucht.name &&
        dort->farbe == gesucht.farbe) {
      cout << "  Richtig!\n";
      punkte++;
    } else {
      cout << "  Leider falsch. Richtig waere z.B. " << gesucht.position
           << ".\n";
    }
  }
  cout << "\nErgebnis: " << punkte << " von " << beantwortet << " Punkten.\n";
}

// ---------------------------------------------------------------------------

int main() {
  Schachbrett brett;
  mt19937 zufall(random_device{}()); // Zufallsgenerator, einmal erstellen

  cout << "=== Schachfiguren raten ===\n";
  cout << "Wie heisst du? ";
  string spieler;
  if (!zeile_lesen(spieler)) {
    return 0;
  }
  if (spieler.empty()) {
    spieler = "Unbekannt";
  }
  cout << "Hallo " << spieler << "!\n";

  string wahl;
  while (true) {
    cout << "\n--- Menue ---\n"
         << "1) Erkunden   - Brett ansehen, Koordinaten nachschlagen\n"
         << "2) Quiz       - Welche Figur steht auf dem Feld?\n"
         << "3) Quiz       - Wo steht die Figur?\n"
         << "q) Beenden\n"
         << spieler << ", deine Wahl: ";

    if (!zeile_lesen(wahl)) {
      break;
    }
    wahl = klein(wahl);

    if (wahl == "1") {
      erkunden(brett);
    } else if (wahl == "2") {
      quiz_welche_figur(brett, zufall);
    } else if (wahl == "3") {
      quiz_wo_steht(brett, zufall);
    } else if (wahl == "q" || wahl == "exit") {
      break;
    } else {
      cout << "Bitte 1, 2, 3 oder q eingeben.\n";
    }
  }

  cout << "Tschuess, " << spieler << "!\n";
  return 0;
}
