//
//  Spielbrett.cpp
//  
//
//  Created by Duc Khuat on 22.05.18.
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
    std::vector<int > vec3(cols*rows*2*4,0) Zähler;
}



//setzt den Zug und aktualisiert Stand[col]. Gibt an, ob jemand gewonnen hat
void Spielbrett::zug(size_t col, size_t spieler){
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

            if ( spieler == 1 )
            {
                std::cout << "Rot gewinnt :) \n Gelb ist ein Loser."  << std::endl;
            }
            else
            {
                std::cout << " Gelb gewinnt :) \n Rot ist ein Loser."  << std::endl;
            }
        }
    }
}


int& Spielbrett::zähler(size_t c, size_t r, int spieler, int type ) //Index für zähler
//spieler 1 oder 2,  type 0 bis 3. (0: Vertikal, 1:Horizontal, 2:DiagR, 3:DiagL)
{
    return Zähler[2*4*c*rows+ 2*4*r + (spieler-1)%2*4 + type];  //eintspricht Zähler(c,r)(spieler)(type), also 4 vectoren.
}
int& Spielbrett::operator () (size_t c, size_t r) //Zugriffsoperator auf das Brett
{
    return Brett[c*rows+ r];
}
void Spielbrett::aktualisiereZähler(size_t c,int spieler ) //betrachte den letzten Zug, welche Spalte und welcher Spieler am Zug war.
{
    size_t r = Stand[c] -1 ; //Zeile des letzten Zuges.
    int d = zähleVert(c,r)-1;
    for (int i = 0; i <d ; i++){
        zähler(c,r-i,spieler, 0)++;
    }
    d = zähleHo(c,r);
}

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
//##################################################################################################################
//              Min-/Maximumalgorithmus
//##################################################################################################################







//lässt die Heuristik auf allen möglichen Spalten laufen und berechnet die Spalte mit dem kleinsten Wert.
// type = 0 tetet alle Felder, type = 1 testet Felder in der Umgebung von c.
size_t MinAlgorithmus(vector<size_t> stand, double& min, int spieler, int type, size_t c, vector<double>& Werte)
{
    
    double heur;
    size_t minI;
    switch (type)
    {
        case 0:                                 //der erste Test. Teste zunächst alle und finde den schlechsteten Zug
                for ( size_t i = 0; i < cols){
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur < min){
                        min = heur;
                        minI = i;
                    }
                }
                break;
        case 1:
            if ( c >= 3 && c+3 < cols)          //überprüfe nur einen eingeschränkten Bereich. Pro gelegter Stein ändert sich maximal etwas in einer Umgebung von 3 Feldern um c etwas.
            {
                for (size_t i = c - 3 ; i < c +3; i++)
                    {
                        if (Stand[i] != rows){
                            heur = heuristik(spieler, i);
                            Werte[i] = heur;
                        }
                        if (heur < min){
                            min = heur;
                            minI = i;
                        }
                    }
            }else if( c >= 3){
                for (size_t i = c -3 ; i < cols; i++)
                {
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur < min){
                        min = heur;
                        minI = i;
                    }
                }
            }else if( c + 3 < cols){
                for( size_t i = 0; i < c+3; i++)
                {
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur < min){
                        min = heur;
                        minI = i;
                    }
                }
            }else{
                for(size_t i=0; i< cols; i++)
                {
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur < min){
                        min = heur;
                        minI = i;
                    }
                }
            }
            break;
    }
    if( alleGleich(Werte) ) return cols/2;
    return minI;
}

size_t MaxAlgorithmus(vector <size_t> stand,double& max, int spieler, int type, size_t c, vector<double>& Werte)
//siehe MinAlgorithmus
{
    
    double heur;
    size_t maxI;
    switch (type)
    {
        case 0:                                 //der erste Test. Teste zunächst alle und finde den besten Zug
            for ( size_t i = 0; i < cols){
                if (Stand[i] != rows){
                    heur = heuristik(spieler, i);
                    Werte[i] = heur;
                    
                }
                if (heur > min){
                    max = heur;
                    maxI = i;
                }
            }
            break;
        case 1:
            if ( c >= 3 && c+3 < cols)          //überprüfe nur einen eingeschränkten Bereich. Pro gelegter Stein ändert sich maximal etwas in einer Umgebung von 3 Feldern um c etwas.
            {
                for (size_t i = c - 3 ; i < c +3; i++)
                {
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur > min){
                        max = heur;
                        maxI = i;
                    }
                }
            }else if( c >= 3){
                for (size_t i = c -3 ; i < cols; i++)
                {
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur > min){
                        max = heur;
                        maxI = i;
                    }
                }
            }else if( c + 3 < cols){
                for( size_t i = 0; i < c+3; i++)
                {
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur > min){
                        max = heur;
                        maxI = i;
                    }
                }
            }else{
                for(size_t i=0; i< cols; i++)
                {
                    if (Stand[i] != rows){
                        heur = heuristik(spieler, i);
                        Werte[i] = heur;
                    }
                    if (heur < min){
                        min = heur;
                        minI = i;
                    }
                }
            }
            break;
    }
    if ( alleGleich(Werte) ) return cols/2;  //praktisch nur der erste Zug.
    return maxI;
}
    
double heuristik (int spieler, size_t c, vector<double>& Werte;)
{
    double res;
    int count = 3;                                  //Rekursionstiefe maximal 4
    size_t r = Stand[c];
    vector <size_t> stand = Stand;                  //Kopie zum Übergeben.
    (*this)(c,r) = spieler;                         //setze testweise Stein von SP und aktualisiert stand.
    stand[c]++;
    vector <int> Umgebung = {zähleVert(c,r),zähleHo(c,r),zähleR(c,r),zähleL(c,r)}; // enthält die Anzahl der gleichfarbigen direkten Nachbarsteine ( direkt heißt, es gibt keine dazwischenliegenden Steine)
    if(Umgebung[0] < 2) && Umgebung[1] <2) && Umgebung[2] <2) && Umgebung[3]<2))
    //falls noch kein anderer Stein in der Umgebung existiert.
    {
        return 0.;
    }
    
    

    res = heuristik2(stand, count, c, spieler, Werte);     //übergebe den nächsten Spieler und die letzte veränderte Spalte.
    (*this)(c,r) = 0;
    stand[c]--;
    return res;
}
double heuristik2(vector<size_t> stand, int count, int c, int spieler, vector<double>& Werte)
// stand, letzter Zug ausgefünrt, count Rekursionstiefe, c die letzte ausgewählte Spalte, spieler von dem aus "heuristik aufgerufen wurde, Werte: Bewertungen der Spalten.
{
    size_t r = stand[I];
    double res;
    double max = 0;
    double min = DBL_MAX;
    
    
    if(checkLine(0,c,r-1) || checkLine(1,c,r-1) || checkLine(2,c,r-1))               //der letzte Zug bringt eine Endstellung.
    {
            return ((count+1)%2)*(-2)+1;                                             //falls count = 3 oder 1, dann war der letzte Zug von SP aus heuristik. Gebe 1 aus. Sonst -1, da GGSP gewinnt.
    }
    switch(count)
    {
        case 3: // hier
            size_t I = MinAlgorithmus(stand, min, spieler%2 +1, 1, c, Werte);               //GGSP ist dran. Liste Werte wird in einer Umgebung von vier Feldern aktualisiert.
            I = minmaxIndex (Werte,0);                                                           //Index des minimalen Wertes der Liste.
            (*this)(I,r) = spieler%2 + 1;
            stand[I]++;
            res = heuristik2 (stand, count -1, I, spieler);
            stand[I]--;
            (*this)(I,r) = 0;
            break;
        case 2:
            size_t I = MaxAlgorithmus(stand, max,spieler, 1, c, Werte);                     //SP ist dran
            I = minmaxIndex (Werte,1);
            (*this)(I,r) = spieler;
            stand[I]++;
            res = heuristik2(stand, count - 1, I, spieler);
            stand[I]--;
            (*this)(I,r) = 0;
            break;
        case 1:
            vector < double> werte = feldwerte( stand, spieler);
            res = minimum(werte);
            break;
    }
    return res;
}
vector<double> felderwerte(vector <size_t stand, int spieler)
{
    vector<double> werte(cols,2); // Defaultwert 2, da alle belegbaren Spalten einen Wert < 2 erhalten.
    for (int i = 0 ; i < cols ; i ++){
        if (stand[i] < rows){
            (*this)(i,stand[i]) = spieler;
            werte[i] = feldwert(i,stand[i]);
            (*this)(i,stand[i]) = 0;
        }else{
            werte[i] = 2;  // da im letzten Schritt, das Minimum gewählt wird, setze nicht mögliche Spalten auf 2.
        }
    }
}
double feldwert(c,r, int spieler){
    if ((*this)(c,r) == spieler){
        vector <int> U= {zähleVert(c,r),zähleHo(c,r),zähleR(c,r),zähleL(c,r)};
        if(checkLine(0,c,r) || checkLine(1,c,r) || checkLine(2,c,r)){ //1 bei Endstellung
            return 1;
        }
        if ( maximum (U) <  3 ){                    //bei maximal 2 benachbarten Steinen
            return 0.2;
        }else if ( U[0] == 3 && U[1]<3 && U[2] < 3 && U[3] <3){ // nur vertikal 3.
            return 0.5;
        }else{
            return 0.7;
        }
    }
}


//#################################################################################################
//                                      HILFSFUNKTIONEN FÜR VEKTOREN
//#################################################################################################
template <Typename T>
size_t minmaxIndex (vector<T>& Werte, int type)    // type = 0 sucht Spalte mit mininmalen Wert,
                                                    // type = 1 sucht Spalte mit maximalen Wert.
{
    size_t res;
    T extrema = Werte[0];
    switch(type){
        case 0:
            for (int i = 1; i < Werte.size(); i++)
            {
                if( Werte[i] < extrema)
                {
                    extrema = Werte[i];
                    res = i;
                }
            }
            break;
        case 1:
            for (int i = 1; i < Werte.size(); i++)
            {
                if( Werte[i] > extrema)
                {
                    extrema = Werte[i];
                    res = i;
                }
            }
            break;
    }
    return res;
}
template <typename T>
T minimum ( std::vector<T>& feld)
{
    T res = feld[0];
    for (int i = 1; i < feld.size(); i++ ){
        if( res > feld[i])
            res = feld[i];
    }
    return res;
}
template <typename T>
T maximum ( std::vector<T>& feld)
{
    T res = feld[0];
    for (int i = 1; i < feld.size(); i++ ){
        if( res < feld[i])
            res = feld[i];
    }
    return res;
}



