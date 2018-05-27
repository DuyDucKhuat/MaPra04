/*******************************************************************
*  Name       : plotx11.h                                          *
*  Verwendung : Schnittstelle zur Grafikbibliothek "plotx11"       *
*  Autor      : J. Bey, V. Reichelt, IGPM RWTH Aachen              *
*  Datum      : Okt '98                                            *
*  Aenderung  : Okt '99                                            *
*  Sprache    : C++                                                *
*******************************************************************/

#ifndef __PLOTX11__
#define __PLOTX11__

// Namen fuer die Farben

enum PlColors
{
  Black, Red, Green, Yellow, Blue, Magenta, Cyan, White, DarkGrey,
  DarkRed, DarkGreen, DarkYellow, DarkBlue, DarkMagenta, DarkCyan, Grey
};

void PlDelay (int msecs);                   // Warte msecs Millisekunden
void PlInit  (int &width, int &height);     // Oeffne Grafik-Fenster,
                                            //   gebe Breite und Hoehe zurueck
void PlClear ();                            // Loesche Grafik-Fenster
void PlFlush ();	                    // Flush X11-Puffer
void PlStop  ();	                    // Beende Grafik
void PlLineColor  (PlColors color);         // Setze Linienfarbe
void PlLineWidth  (int width);              // Setze Linienbreite
void PlMarkerSize (int width);              // Setze Linienbreite

// Definiere Fensterausschnitt, und lege Koordinatensystem darin fest

void PlFrame (int x0, int x1, int y0, int y1);
void PlScale (double x0, double x1, double y0, double y1);

// Zeichne Marker und/oder Linie je nach "mode":
// mode<0: Gehe nach (x,y) und setze Marker
// mode>0: Zeichne Linie bis (x,y) und setze Marker
// mode=0: Tue nichts
// Der Markertyp haengt von abs(mode) ab:
// case 1: Kein Marker
// case 2: +
// case 3: Rhombus
// case 4: Quadrat
// case 5: Dreieck
// case 6: x
// case 7: Kreis
// case 7: Kreisscheibe

void PlPlot (double x, double y, int mode);

// Gebe aktuelle Koordinaten zurueck

void PlWhere (double &x, double &y);
void HandleXEvents();

#endif
