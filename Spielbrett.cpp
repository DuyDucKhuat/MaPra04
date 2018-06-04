//
//  SP.cpp
//  
//
//  Created by Duc Khuat on 04.06.18.
//

#include "Spielbrett.h"

// =======================
//      Konstruktoren
// =======================

//spieler 1 ist Rot. spieler 2 ist Gelb.
// Sei GGSP Gegenspieler von SP Spieler.
Spielbrett::Spielbrett(size_t cols, size_t rows){
    std::vector<int> vec(cols*rows,0);
    Brett = vec;
    std::vector<size_t> vec2(cols,0);
    Stand = vec2;    
}

// ============================================================================================
//                  SPIELBRETT
// ============================================================================================

//setzt den Zug und aktualisiert Stand[col]. Gibt an, ob jemand gewonnen hat
void Spielbrett::zug(size_t col, size_t spieler, bool& ende){
    if (Stand[col] == rows){
        std::cout << " ungültiger Zug, bitte wähle eine andere Spalte, Spalte ist bereits voll " << std::endl;
    }else{
        if ( spieler == 1){
            Brett[col*rows + Stand[col]] = 1;
        }else if( spieler == 2){
            Brett[col*rows + Stand[col]] = 2;
        }
        Stand[col]++; //aktualisiere Stand.
        if( checkLine (0,col,Stand[col]) || checkLine (1,col,Stand[col]) || checkLine (2,col,Stand[col])){
            // Annahme ist, dass vorher das Spiel noch nicht zu einem Ende gekommen ist. Deswegen hat hier der ziehende Spieler gewonnen.
            ende = true;
        }
    }
}



int& Spielbrett::operator () (size_t c, size_t r) //Zugriffsoperator auf das Brett
{
    return Brett[c*rows+ r];
}




// ############################################################################################################
//                      CHECK LINE
// ############################################################################################################

int zähleVert(size_t c, size_t r) // zählt in beide Richtungen vertikal.
{
    size_t i = r;
    int sum = 1;
    int player = (*this)(c,r);
    size_t max = 0;
    max--;
    while ((*this)(c,i+1) == player && i+1 < rows){
        sum++;
        i++;
    }
    i = r;
    while ((*this)(c,i-1) == player && i-1 != max){ //SOLANGE i nicht null ist.
        sum++;
        i--;
    }
    return sum;
}
int zähleHo(size_t c, size_t r) // zählt in beide Richtungen horizontal.
{
    size_t i = c;
    int sum = 1;
    int player = (*this)(c,r);
    size_t max = 0;
    max--;
    
    while((*this)(i-1,r) == player && i-1 != max){
        sum++;
        i--;
    }
    i = c;
    while((*this)(i+1,r) == player && i+1 < rows){
        sum++;
        i++;
    }
    return sum;
}

int zähleDiagR( size_t c, size_t r) //Diagonale in beide Richtungen von unten links nach oben rechts.

{
    int player = (*this)(c,r);
    int sum = 1;
    size_t max =0;
    size_t d =0;
    max--;
    while((*this)(c+d+1,r+d+1) == player && c+d+1 < cols && r+d+1 < rows){
        sum++;
        d++;
    }
    d = 0;
    while ((*this)(c-(d+1),r-(d+1)) == player &&c-(d+1) != max && r - (d+1) != max){
        sum ++;
        d++;
    }
    return sum;
}

int zähleDiagL( size_t c, size_t r) // Diagonale in beide Richtungen von oben links nach unten rechts.
{
    size_t d =0;
    int player = (*this)(c,r);
    int sum = 1;
    size_t max =0;
    max--;
    while((*this)(c-(d+1),r+d+1) == player && c-(d+1) != max && r+d+1 < rows){ //Richtung: nach links oben.
        sum++;
        d++;
    }
    d = 0;
    while((*this)(c+(d+1),r-(d+1)) == player && c+(d+1) < cols && r-(d+1) != max){ //Richtung: nach rechts unten.
        sum++;
        d++;
    }
    return sum;
}
bool Spielbrett::checkLine(int type, size_t c, size_t r)
{
    //überprüft, ob eine Endstellung bei einem belegten Stein erreicht wurde.
    int player = (*this)(c,r);
    if( player == 0) return false;
    switch (type){
        case 0:  //vertikal
            if (zähleVert(c,r) >= 4) {return true;}
            else {return false;}
            break;
        case 1: //horizontal
            if( zähleHo(c,r) >= 4){return true;}
            else {return false;}
            break;
        case 2://diagonal
            if( zähleDiagR(c,r) >= 4 || zähleDiagL(c,r) >= 4){return true;}
            else {return false;}
            break;
    }
}





//##############################################################################################################
//              Maximumalgorithmus
//##################################################################################################################
double wertung(size_t c){
    double res = 0;
    for ( int i = 0; i < cols-3 ; i++){ //vertikal
        for ( int j = 0; j < rows; j++){
            if (i < cols -3) res += feldabschnitt(i,j,0);
            if (j >= 3 ) res+=feldabschnitt(i,j,1);
            if (i < cols -3 && j < rows -3) res+= feldabschnitt(i,j,2);
            if (i >= 3 && j>=3) res+= feldabschnitt(i,j,3);
        }
    }
    return res;
}
size_t nächsterZug(){ //für Spieler 1
    size_t res;
    maxAlgorithmus (4,res);
    return res;
}
size_t maxAlgorithmus(int recursion, size_t& maxI)
{
    double heur;
    double max = -1.;
    for ( size_t i = 0; i < cols){
        if (Stand[i] != rows){
            (*this)(i,Stand[i]) = 1;
            Stand[i]++;
            if(checkLine(0,c,Stand[i]-1) || checkLine(1,c,Stand[i]-1) || checkLine(2,c,Stand[i]-1)){
                heur = 1.;
            }
            else if(umgebungNull(i)){
                heur =0.;
            }else{
                if ( recursion > 1) heur = minAlgorithmus(recursion - 1);
                else heur = wertung(i);
            }
            (*this)(i,Stand[i]) = 0;
            Stand[i]--;
        }
        if (heur > min){
            max = heur;
            maxI = i;
        }
    }
    return max;
}

size_t minAlgorithmus(int recursion, size_t& minI)
{
    double heur;
    double min = 2.;
    for ( size_t i = 0; i < cols){
        if (Stand[i] != rows){
            (*this)(i,Stand[i]) = 2;
            Stand[i]++;
            if(checkLine(0,c,Stand[i]-1) || checkLine(1,c,Stand[i]-1) || checkLine(2,c,Stand[i]-1)){
                heur = -.1;
            }
            else{
                if ( recursion > 1) heur = maxAlgorithmus(recursion - 1);
                else heur = wertung(i);
            }
            Stand[i]--;
            (*this)(i,Stand[i]) = 0;
        }
        if (heur < min){
            min = heur;
            minI = i;
        }
    }
    return min;
}


//##################################################################################################################
//                  HILFSFUNKTIONEN
//##################################################################################################################

bool umgebungNull(size_t c){
    size_t r = Stand[c-1];
    vector <int> Umgebung = {zähleVert(c,r),zähleHo(c,r),zähleR(c,r),zähleL(c,r)}; // enthält die Anzahl der gleichfarbigen direkten Nachbarsteine ( direkt heißt, es gibt keine dazwischenliegenden Steine)
    return Umgebung[0] < 2) && Umgebung[1] <2) && Umgebung[2] <2) && Umgebung[3]<2
}
//überprüft, ob alle Werte gleich sind, in diesem Fall wird zufällig ein Zug gewählt.
tepmplate <Typename T>
bool alleGleich (std::vector <T> Werte){
    bool res = true;
    double start = Werte[0];
    for ( int i = 1; i  < Werte.size(); i++){
        if (Werte[i] != start )
            res = false;
    }
    return res;
    
}


double feldabschnitt( size_t c, size_t r, int type)
{
    int ersterStein = -1; // defaultWert
    int count = 0;

    switch ( type){ // es steht vier Mal fast das Gleiche da.
        case 0: //vertikal von (c,r) ausgehend.
            if( c > cols -3 ) std::cout << "kein möglicher feldabschnitt. 0" << std::endl;
            
            vector <int> vec = {(*this)(c,r),(*this)(c+1,r),(*this)(c+2,r),(*this)(c+3,r)};
            bool alleGleich = alleGleich(vec);
            if ( alleGleich && vec[0] == 0)  return 0.; //alle vier Felder sind unbesetzt.
            for( int i = 0; i < 4; i++){
                if ( (*this)(c+i,r) !=0 && ersterStein == -1 ) ersterStein = (*this)(c+i,r);
                if( (*this(c+i,r) != 0)  && ersterStein != (*this(c+i,r)) return 0.;
            }
            //jetzt der Fall in dem keine unterschiedlichen Farben enthalten sind.
            int count = 1;
            for( int i = 0; i < 4; i++){
                if ( ersterStein == (*this(c+i,r) ) count++;
            }
            switch (count){
                case 1:
                    if( ersterStein ==1) return 0.1;
                    else return -0.1;
                    break;
                case 2:
                    if( ersterStein ==1) return 0.3;
                    else return -0.3;
                    break;
                case 3:
                    if( ersterStein ==1) return 0.7;
                    else return -0.7;
                    break;
            }
            break;
        case 1: // horizontal
                if( r >= 3) std::cout << "kein möglicher feldabschnitt. 1" << std::endl;
                vector <int> vec = {(*this)(c,r),(*this)(c,r-1),(*this)(c,r-2),(*this)(c,r-3)};
                bool alleGleich = alleGleich(vec);
                if( alleGleich && vec[0] == 0) return 0.
                for( int i = 0; i < 4; i++){
                    if ( (*this)(c,r-i) !=0 && ersterStein == -1 ) ersterStein = (*this)(c,r-i);
                    if( (*this(c,r-i) != 0)  && ersterStein != (*this(c,r-i)) return 0.;
                }
                for( int i = 0; i < 4; i++){
                    if ( ersterStein == (*this(c,r-i) ) count++;
                }
                switch (count){
                    case 1:
                        if( ersterStein ==1) return 0.1;
                        else return -0.1;
                        break;
                    case 2:
                        if( ersterStein ==1) return 0.3;
                        else return -0.3;
                        break;
                    case 3:
                        if( ersterStein ==1) return 0.7;
                        else return -0.7;
                        break;
                }
                break;
        case 2: //DiagR
            if(c >= cols - 3 || r >= cols - 3 ) std::cout << "kein möglicher feldabschnitt. 1" << std::endl;
            vector <int> vec = {(*this)(c,r),(*this)(c+1,r+1),(*this)(c+2,r+2),(*this)(c+3,r+3)};
            bool alleGleich = alleGleich(vec);
            if( alleGleich && vec[0] == 0) return 0.
            for( int i = 0; i < 4; i++){
                if ( (*this)(c+i,r+i) !=0 && ersterStein == -1 ) ersterStein = (*this)(c+i,r+i);
                if( (*this(c+i,r+i) != 0)  && ersterStein != (*this(c+i,r+i)) return 0.;
            }
            for( int i = 0; i < 4; i++){
                if ( ersterStein == (*this(c+i,r+i) ) count++;
            }
            switch (count){
                case 1:
                    if( ersterStein ==1) return 0.1;
                    else return -0.1;
                    break;
                case 2:
                    if( ersterStein ==1) return 0.3;
                    else return -0.3;
                    break;
                case 3:
                    if( ersterStein ==1) return 0.7;
                    else return -0.7;
                    break;
                }
        case 3: //DiagL
            if(c >= cols - 3 || r < 3 ) std::cout << "kein möglicher feldabschnitt. 1" << std::endl;
            vector <int> vec = {(*this)(c,r),(*this)(c+1,r-1),(*this)(c+2,r-2),(*this)(c+3,r-3)};
            bool alleGleich = alleGleich(vec);
            if( alleGleich && vec[0] == 0) return 0.
            for( int i = 0; i < 4; i++){
                if ( (*this)(c+i,r-i) !=0 && ersterStein == -1 ) ersterStein = (*this)(c+i,r-i);
                if( (*this(c+i,r-i) != 0)  && ersterStein != (*this(c+i,r-i)) return 0.;
            }
            for( int i = 0; i < 4; i++){
                if ( ersterStein == (*this(c+i,r-i) ) count++;
            }
            switch (count){
                case 1:
                    if( ersterStein ==1) return 0.1;
                    else return -0.1;
                    break;
                case 2:
                    if( ersterStein ==1) return 0.3;
                    else return -0.3;
                    break;
                case 3:
                    if( ersterStein ==1) return 0.7;
                    else return -0.7;
                    break;
            }
    }
}

