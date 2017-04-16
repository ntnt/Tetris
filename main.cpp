#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <sstream>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "resource/resource.h"
using namespace sf;
using namespace std;
const int M = 20;
const int N = 10;
bool paused = 0;
bool fail = 0;
bool zaczelo_sie = 0;
int field[M][N] = {0};
struct Point {int x,y;} a[4], b[4], c[4];
fstream hscoreSource;
int figures[7][4] =
{
 /////////
 // 0,1 //
 // 2,3 //
 // 4,5 //
 // 6,7 //
 /////////
 1,3,5,7, // I
 2,4,5,7, // Z
 3,5,4,6, // S
 3,5,4,7, // T
 2,3,5,7, // L
 3,5,7,6, // J
 2,3,4,5, // O
};
void GameOver(RenderWindow& window, Music& m, Text& t, RectangleShape& gameoverbg);

// sprawdzenie kolizji ze œciankami
bool check()
{
   for (int i=0;i<4;i++)
   if (a[i].x<0 || a[i].x>=N || a[i].y>=M || field[a[i].y][a[i].x])
        return 1;
   return 0;
};

int main()
{
    hscoreSource.open("hscore/hscore.txt", ios::in | ios::out);
// Image Icon;
// if (!Icon.loadFromFile("resource/icon.ico"))
//		return EXIT_FAILURE;

 Music muzyczka;
    if (!muzyczka.openFromFile("sounds/muzyczka.ogg"))
 {
 return EXIT_FAILURE;
 }

 SoundBuffer bufor;
 bufor.loadFromFile("sounds/line.wav");
 Sound linia(bufor);

 bool music_on = false;

 muzyczka.setLoop(1);
 muzyczka.setVolume(15);
 muzyczka.play();

//  ladowanie czcionki
 Font czciona;
 if (!czciona.loadFromFile("images/BGOTHL.TTF"))
 {
  return EXIT_FAILURE;
 }
 int score = 0;
 std::stringstream ss, ss2;
 stringstream punkty;

 Text txtScore, txtGameOver, txtHScore;
 // background
 RectangleShape menubg, gameoverbg;
 sf::Vector2f nowy(100, 50);
 menubg.setFillColor(Color(0,0,0, 100));
 menubg.setPosition(240, 30);
 menubg.setSize(nowy);
 gameoverbg.setFillColor(Color(0,0,0,150));
 gameoverbg.setPosition(10,200);
 gameoverbg.setSize(Vector2f(300, 100));

 // ***
 txtScore.setString(ss.str());
 txtScore.setFont(czciona);
 txtScore.setScale(0.7f,0.7f);
 txtScore.setPosition(245, 35);

 txtHScore.setString(ss.str());
 txtHScore.setFont(czciona);
 txtHScore.setScale(0.7f,0.7f);
 txtHScore.setPosition(245, 95);
 txtGameOver.setString("Przegrana!\n Pusia wygrywa!");
 txtGameOver.setFont(czciona);
 txtGameOver.setPosition(20,210);
 //////////////
    srand(time(0));
 RenderWindow window(VideoMode(320, 480), "Haletris v0.2");
// window.setIcon(32,32, Icon.getPixelsPtr());
    Texture t1,t2,t3;
 t1.loadFromFile("images/tiles.png");
 t2.loadFromFile("images/background.png");
 t3.loadFromFile("images/frame.png");
 Sprite s(t1), background(t2), frame(t3);
    int dx=0; bool rotate=0; int colorNum[2] {4,4};
    int speeder = 0;
 float timer=0,delay=1.5, delay2 = 1.5;
 Clock clock;
 float time;

 colorNum[1] = 1 + rand() % 7;
int n[2] {rand() % 7, rand() % 7};
    for (int i = 0; i<4; i++)
    {
     // 3, 5, 4, 7, // T
     a[i].x = (figures[n[0]][i] % 2)+3;
     a[i].y = figures[n[0]][i] / 2;
     c[i].x = (figures[n[1]][i] % 2)+3;
     c[i].y = figures[n[1]][i] / 2;
     }
    n[1] = rand() % 7;
    n[0] = n[1];
    while (window.isOpen())
    {
  time = clock.getElapsedTime().asSeconds();
  clock.restart();
  timer+=time;
    Event e;
    while (window.pollEvent(e))
    {
        if (e.type == Event::Closed)
            window.close();
        if (e.type == Event::KeyPressed)
        {
            if (e.key.code==Keyboard::W) rotate=true;
            else if (e.key.code==Keyboard::A) dx=-1;
            else if (e.key.code==Keyboard::D) dx=1;
            else if (e.key.code == Keyboard::E) {score++; speeder++;}
            else if (e.key.code == Keyboard::P) paused = !paused;
            else if (e.key.code == Keyboard::Escape) window.close();
            else if (e.key.code == Keyboard::M)
            {
                if (!music_on) muzyczka.play();
                else muzyczka.stop();
                music_on = !music_on;
            }
        }
    }
 if (Keyboard::isKeyPressed(Keyboard::S)) {delay2 = delay; delay=0.05; }
 //// <- Move -> ///
 for (int i = 0; i < 4; i++) {
  b[i] = a[i]; a[i].x += dx;
 }
    if (check())  for (int i=0;i<4;i++) a[i]=b[i];
 //////Rotate//////
 if (rotate)
   {
  Point p = a[1]; //center of rotation
  for (int i=0;i<4;i++)
    {
   int x = a[i].y-p.y;
   int y = a[i].x-p.x;
   a[i].x = p.x - x;
   a[i].y = p.y + y;
     }
        if (check()) for (int i=0;i<4;i++) a[i]=b[i];
   }
 ///////Tick//////
 if (!paused)
 {
  if (timer>delay)
  {
   for (int i = 0; i<4; i++) { b[i] = a[i]; a[i].y += 1; }
   if (check())
   {
    for (int i = 0; i<4; i++) field[b[i].y][b[i].x] = colorNum[0];
    colorNum[0] = colorNum[1];
    colorNum[1] = 1 + rand() % 7;

        n[1] = rand() % 7;

    for (int i = 0; i<4; i++)
    {
     // 3, 5, 4, 7, // T
     a[i].x = (figures[n[0]][i] % 2)+3;
     a[i].y = figures[n[0]][i] / 2;
     c[i].x = (figures[n[1]][i] % 2)+3;
     c[i].y = figures[n[1]][i] / 2;
     if (field[a[i].y][a[i].x]) { fail = 1; }
    } // 1,1  1,2  0,2  1,3
        n[0] = n[1];
   }
   timer = 0;
  }
 }
 else { timer = 0; }


 ///////check lines//////////
    int k=M-1;
 for (int i=M-1;i>0;i--)
 {
  int count = 0;
  for (int j =  0;j < N; j++)
  {
   if (field[i][j]) { count++; }
      field[k][j] = field[i][j];
  }
  if (count < N) { k--;   }
  else if (count == N) { score++; speeder++; linia.play(); }
 }

    dx = 0; rotate = 0;
    if (speeder >= 10)
     { delay2 -= 0.1f; speeder = 0; }
     delay = delay2;


    ///////// draw //////////
    window.clear(Color::White);
    window.draw(background);

 for (int i=0;i<M;i++)
  for (int j=0;j<N;j++)
    {
         if (field[i][j] == 0) continue;
   s.setTextureRect(IntRect(field[i][j]*18,0,18,18));
   s.setPosition(j*18,i*18);
   s.move(28,31); //offset
   window.draw(s);
    }

 for (int i=0;i<4;i++)
   {
  s.setTextureRect(IntRect(colorNum[0]*18,0,18,18));
  s.setPosition(a[i].x*18,a[i].y*18);
  s.move(28,31); //offset
  window.draw(s);

  s.setTextureRect(IntRect(colorNum[1]*18,0,18,18));
  s.setPosition(c[i].x*18+170,c[i].y*18+130);
  s.move(28,31); //offset
  window.draw(s);

   }
// ss << score;
 window.draw(frame);
 txtScore.setString(ss.str());
 window.draw(menubg);
 ss.str(std::string()); ss << std::setfill('0') << std::setw(6) << score*10;
 window.draw(txtScore);
 if (fail == 1) GameOver(window, muzyczka, txtGameOver, gameoverbg);

  window.display();
 }
    return 0;
}

void GameOver(RenderWindow& window, Music& m, Text& t, RectangleShape& gameoverbg)
{
     m.stop();
    window.draw(gameoverbg);
    window.draw(t);
    window.display();
    sleep(seconds(3));
    window.close();
}
