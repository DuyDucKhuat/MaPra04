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
    
    Spielbrett::Spielbrett(size_t cols, size_t rows){
        std::vector<int> vec(cols*rows,0);
        Brett = vec;
        std::vector<int> vec2(cols,0);
        Stand = vec2;
    }


    void Spielbrett::zug(size_t col, size_t spieler){
        if (Stand[col] == rows){
            std::cout << " ungültiger Zug, bitte wähle eine andere Spalte, Spalte ist bereits voll " << std::endl;
        }else{
            if ( spieler == 1){
                Brett[col*rows + Stand[col]] = 1;
            }else if( spieler == 2){
                Brett[col*rows + Stand[col]] = 2;
                
            }
            
            Stand[col]++;
            if( checkLine (0,col,Stand[col]) || checkLine (1,col,Stand[col]) || checkLine (2,col,Stand[col])){
                if ( spieler == 1 )
                    std::cout << "Rot gewinnt :) \n Gelb ist ein Loser."  << std::endl;
                else
                    std::cout << " Gelb gewinnt :) \n Rot ist ein Loser."  << std::endl;
            }
        }
    }
    int& Spielbrett::operator () (size_t c, size_t r){
        return Brett[c*rows+ r];
    }
    
bool Spielbrett::checkLine(int type, size_t c, size_t r)
{ // überprüft, ob eine Endstellung bei einem belegten Stein erreicht wurde.
        int player = (*this)(c,r);
        if( player == 0) return false;
        size_t i,d, sum, sum2;
        size_t max = 0;
        max--; // ist jetzt die größte size_t Zahk
        
        switch (type){
            case 0:  //vertikal
                i = r;
                sum = 0;
                while ((*this)(c,i+1) == player && i+1 < rows){
                    sum++;
                    i++;
                }
                i = r;
                while ((*this)(c,i-1) == player && i-1 != max){
                    sum++;
                    i--;
                }
                if (sum >= 4) {return true;}
                else {return false;}
                break;
            case 1: //horizontal
                i = c;
                sum = 0;
                while((*this)(i-1,r) == player && i-1 != max){
                    sum++;
                    i--;
                }
                i = c;
                while((*this)(i+1,r) == player && i+1 < rows){
                    sum++;
                    i++;
                }
                if( sum >= 4){return true;}
                else {return false;}
                break;
            case 2://diagonal
                d = 0;
                sum = 0;
                sum2 = 0;
                while((*this)(c+d+1,r+d+1) == player && c+d+1 < cols && r+d+1 < rows){
                    sum++;
                    d++;
                }
                d = 0;
                while ((*this)(c-(d+1),r-(d+1)) == player &&c-(d+1) != max && r - (d+1) != max){
                    sum ++;
                    d++;
                }
                d = 0;
                while((*this)(c-(d+1),r+d+1) == player && c-(d+1) != max && r+d+1 < rows){
                    sum2++;
                    d++;
                }
                d = 0;
                while((*this)(c+(d+1),r-(d+1)) == player && c+(d+1) < cols && r-(d+1) != max){
                    sum2++;
                    d++;
                }
                if( sum >= 4 || sum2 >= 4){return true;}
                else {return false;}
                break;
        }
}
