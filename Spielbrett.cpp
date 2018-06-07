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
    this->cols = cols;
    this->rows = rows;
}

// ============================================================================================
//                  SPIELBRETT
// ============================================================================================

//setzt den Zug und aktualisiert Stand[col]. Gibt an, ob jemand gewonnen hat
bool Spielbrett::BrettVoll(){
    bool voll = true;
    for (int i = 0 ; i < cols ; i++){
        if (Stand[i] < rows) voll = false;
    }
    return voll;
}
void Spielbrett::zug(size_t col, size_t spieler, int& weWin ){
    if ( col >= cols) std::cout << "ungueltige Spalte" << std::endl;
    else{
        if (Stand[col] == rows){
            std::cout << Stand[col] << " " << rows << std::endl;
            std::cout << " ungueltiger Zug, bitte wähle eine andere Spalte, Spalte ist bereits voll " << std::endl;
        }else{
            if ( spieler == 1){
                Brett[col*rows + Stand[col]] = 1;
            }else if( spieler == 2){
                Brett[col*rows + Stand[col]] = 2;
            }
            Stand[col]++; //aktualisiere Stand.
            if ( checkLine(0,col, Stand[col]-1 ) || checkLine(1,col, Stand[col]-1 )|| checkLine(2,col,  Stand[col]-1 ))
            {
                if ( spieler ==1) weWin = 1;
                else weWin = -1;
            }
        }
    }
}



int& Spielbrett::operator () (size_t c, size_t r) //Zugriffsoperator auf das Brett
{
    return Brett[c*rows+ r];
}
int Spielbrett::operator ()  (size_t c, size_t r) const
{
    return Brett[c*rows+ r];
}



// ############################################################################################################
//                      CHECK LINE
// ############################################################################################################

int Spielbrett::zaehleVert(size_t c, size_t r) // zählt in beide Richtungen vertikal.
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
int Spielbrett::zaehleHo(size_t c, size_t r) // zählt in beide Richtungen horizontal.
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

int Spielbrett::zaehleDiagR( size_t c, size_t r) //Diagonale in beide Richtungen von unten links nach oben rechts.

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

int Spielbrett::zaehleDiagL( size_t c, size_t r) // Diagonale in beide Richtungen von oben links nach unten rechts.
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
            if (zaehleVert(c,r) >= 4) {return true;}
            else {return false;}
            break;
        case 1: //horizontal
            if( zaehleHo(c,r) >= 4){return true;}
            else {return false;}
            break;
        case 2://diagonal
            if( zaehleDiagR(c,r) >= 4 || zaehleDiagL(c,r) >= 4){return true;}
            else {return false;}
            break;
    }
    return false;
}



//##############################################################################################################
//              Maximumalgorithmus
//##################################################################################################################
double Spielbrett::wertung(){
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
size_t Spielbrett::naechsterZug(){ //für Spieler 1
    size_t res;
    srand (time(NULL));
    std::cout <<maxAlgorithmus (4,res) << std::endl;
    return res;
}
double Spielbrett::maxAlgorithmus(int recursion, size_t& maxI)
{

    double heur;
    double max = -1.;
    for ( size_t i = 0; i < cols; i++){
        if (Stand[i] != rows){
            (*this)(i,Stand[i]) = 1;
            Stand[i]++;
            if(checkLine(0,i,Stand[i]-1) || checkLine(1,i,Stand[i]-1) || checkLine(2,i,Stand[i]-1)){
                heur = 1.;
            }
            else if(umgebungNull(i)){
                heur = 0.;
            }else{
                if ( recursion > 1) heur = minAlgorithmus(recursion - 1, maxI);
                else {heur = wertung();}
            }
            Stand[i]--;
            (*this)(i,Stand[i]) = 0;
        }
        if (heur > max){
            max = heur;
            maxI = i;
        }
        if ( maxI == 0) maxI = std::rand() % cols;
    }


    return max;
}

double Spielbrett::minAlgorithmus(int recursion, size_t& minI)
{

    double heur;
    double min = 2.;
    for ( size_t i = 0; i < cols; i++){

        if (Stand[i] != rows){
            (*this)(i,Stand[i]) = 2;
            Stand[i]++;
            if(checkLine(0,i,Stand[i]-1) || checkLine(1,i,Stand[i]-1) || checkLine(2,i,Stand[i]-1)){
                heur = -1.;
            }
            else{
                if ( recursion > 1) heur = maxAlgorithmus(recursion - 1, minI);
                else {heur = wertung();

                }
            }
            Stand[i]--;
            (*this)(i,Stand[i]) = 0;
        }


        if (heur < min){
            min = heur;
            minI = i;
        }
        if ( minI == 0) minI = std::rand() % cols;


    }

    
    return min;
}


//##################################################################################################################
//                  HILFSFUNKTIONEN
//##################################################################################################################

bool Spielbrett::umgebungNull(size_t c){

    size_t r = Stand[c]-1;
    std::vector <int> Umgebung = {zaehleVert(c,r),zaehleHo(c,r),zaehleDiagR(c,r),zaehleDiagL(c,r)}; // enthält die Anzahl der gleichfarbigen direkten Nachbarsteine ( direkt heißt, es gibt keine dazwischenliegenden Steine)
    return ((Umgebung[0] < 2) && (Umgebung[1] <2) && (Umgebung[2] <2) && (Umgebung[3]<2));
}
//überprüft, ob alle Werte gleich sind, in diesem Fall wird zufällig ein Zug gewählt.
template <typename T>
bool Spielbrett::alleGleich (std::vector <T> Werte){
    bool res = true;
    double start = Werte[0];
    for ( int i = 1; i  < Werte.size(); i++){
        if (Werte[i] != start )
            res = false;
    }
    return res;
    
}


double Spielbrett::feldabschnitt( size_t c, size_t r, int type)
{

    int ersterStein = -1; // defaultWert
    int count = 0;
    std::vector <int> vec;


    switch ( type){ // es steht vier Mal fast das Gleiche da.
        case 0: //vertikal von (c,r) ausgehend.
            if( c > cols -3 ) std::cout << "kein möglicher feldabschnitt. 0" << std::endl;
            vec = {(*this)(c,r),(*this)(c+1,r),(*this)(c+2,r),(*this)(c+3,r)};
            if ( alleGleich(vec) && vec[0] == 0)  return 0.; //alle vier Felder sind unbesetzt.
            for( int i = 0; i < 4; i++){
                if (( (*this)(c+i,r) !=0) && (ersterStein == -1 )) ersterStein = (*this)(c+i,r);
                
                if( ((*this)(c+i,r) != 0)  && (ersterStein != (*this)(c+i,r))) return 0.;
            }
            //jetzt der Fall in dem keine unterschiedlichen Farben enthalten sind.
            
            for( int i = 0; i < 4; i++){
                if ( ersterStein == (*this)(c+i,r) ) count++;
            }
            switch (count){
                case 1:
                    if( ersterStein ==1) return 0.001;
                    else return -0.001;
                    break;
                case 2:
                    if( ersterStein ==1) return 0.01;
                    else return -0.01;
                    break;
                case 3:
                    if( ersterStein ==1) return 0.05;
                    else return -0.05;
                    break;
            }
            break;
        case 1: // horizontal
                if( r < 3) std::cout << "kein möglicher feldabschnitt. 1" << std::endl;
                vec = {(*this)(c,r),(*this)(c,r-1),(*this)(c,r-2),(*this)(c,r-3)};
                if( alleGleich(vec) && vec[0] == 0) return 0.;
                for( int i = 0; i < 4; i++){
                    if ( ( (*this)(c,r-i) !=0) && (ersterStein == -1 )) ersterStein = (*this)(c,r-i);
                    if( ((*this)(c,r-i) != 0)  && (ersterStein != (*this)(c,r-i))) return 0.;
                }
                for( int i = 0; i < 4; i++){
                    if ( ersterStein == (*this)(c,r-i) ) count++;
                }
                switch (count){
                    case 1:
                        return 0.;
                        break;
                    case 2:
                        if( ersterStein ==1) return 0.01;
                        else return -0.01;
                        break;
                    case 3:
                        if( ersterStein ==1) return 0.05;
                        else return -0.05;
                        break;
                }
                break;
        case 2: //DiagR
            if(c >= cols - 3 || r >= cols - 3 ) std::cout << "kein möglicher feldabschnitt. 1" << std::endl;
            vec = {(*this)(c,r),(*this)(c+1,r+1),(*this)(c+2,r+2),(*this)(c+3,r+3)};
            if( alleGleich(vec) && vec[0] == 0) return 0.;
            for( int i = 0; i < 4; i++){
                if ( ((*this)(c+i,r+i) !=0) && (ersterStein == -1 )) ersterStein = (*this)(c+i,r+i);
                if( ((*this)(c+i,r+i) != 0)  && (ersterStein != (*this)(c+i,r+i))) return 0.;
            }
            for( int i = 0; i < 4; i++){
                if ( ersterStein == (*this)(c+i,r+i) ) count++;
            }
            switch (count){
                case 1:
                    return 0.;
                    break;
                case 2:
                    if( ersterStein ==1) return 0.01;
                    else return -0.01;
                    break;
                case 3:
                    if( ersterStein ==1) return 0.05;
                    else return -0.05;
                    break;
                }
        case 3: //DiagL
            if(c >= cols - 3 || r < 3 ) std::cout << "kein möglicher feldabschnitt. 1" << std::endl;
            vec = {(*this)(c,r),(*this)(c+1,r-1),(*this)(c+2,r-2),(*this)(c+3,r-3)};
            if( alleGleich(vec) && vec[0] == 0) return 0.;
            for( int i = 0; i < 4; i++){
                if (( (*this)(c+i,r-i)) !=0 && (ersterStein == -1)) ersterStein = (*this)(c+i,r-i);
                if( ((*this)(c+i,r-i) != 0)  && (ersterStein != (*this)(c+i,r-i))) return 0.;
            }
            for( int i = 0; i < 4; i++){
                if ( ersterStein == (*this)(c+i,r-i) ) count++;
            }
            switch (count){
                case 1:
                    return 0.;
                    break;
                case 2:
                    if( ersterStein ==1) return 0.001;
                    else return -0.001;
                    break;
                case 3:
                    if( ersterStein ==1) return 0.05;
                    else return -0.05;
                    break;
            }
    }
    return 0.;
}
void Spielbrett::anzeige(){
    for (int i = 0; i <rows ; i++){
        for( int j = 0; j < cols ; j++){
            std::cout << (*this)(j,7-i) << " ";
        }
        std::cout << "\n";
    }
}
void spiele(Spielbrett& A){
    bool b = true;
    size_t c, c2;
    while (b){
        for (int i = 0; i <A.rows ; i++){
            for( int j = 0; j < A.cols ; j++){
                std::cout << A(j,7-i) << " ";
            }
            std::cout << "\n";
        }
    std::cout << A.naechsterZug() << std::endl;
    std::cin >> c  >> c2  >> b;
    int win;
    A.zug(c,1,win);
    A.zug(c2,2,win);
    
    }
        
        
}
