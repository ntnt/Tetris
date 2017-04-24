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
bool only_I = 0;
int field[M][N] = {0};
int highScore = 0;
int multi;
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
bool check();
template <typename T, size_t N>
void Init(T (&a)[N]);
void KeyInput(Event& e);
void Update();
Music muzyczka;
 SoundBuffer bufor;
 Sound linia(bufor);
 bool music_on = false;
 Font czciona;
 int score = 10;
 Event e;
 std::stringstream ss, ss2;

 Text txtScore, txtGameOver, txtHScore;
 RectangleShape menubg, gameoverbg;
 sf::Vector2f nowy(100, 50);

 RenderWindow window(VideoMode(320, 480), "Haletris v0.2");
// window.setIcon(32,32, Icon.getPixelsPtr());
    Texture t1,t2,t3;
    Clock sfclock;
    int dx=0; bool rotated=0; int colorNum[2] {4,4};
    int speeder = 0;
 float timer=0,delay=1.5, delay2 = 1.5;
     int n[2] {rand() % 7, rand() % 7};
 float timed;
int main()
{
    srand(time(0));
// Image Icon;
// if (!Icon.loadFromFile("resource/icon.ico"))
//		return EXIT_FAILURE;
    Init(n);
    Sprite s(t1), background(t2), frame(t3);
    while (window.isOpen())
    {
    KeyInput(e);
    Update();
 //// <- Move -> ///
 for (int i = 0; i < 4; i++) {
  b[i] = a[i]; a[i].x += dx;
 }
    if (check())  for (int i=0;i<4;i++) a[i]=b[i];
 //////Rotate//////
 if (rotated)
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


 ///////check lines//////////
 int k = M - 1;
 for (int i = M-1 ; i>0 ;i--)
 {
  int count = 0;
  for (int j =  0;j < N; j++)
  {
   if (field[i][j]) { count++; }
      field[k][j] = field[i][j];
  }

  if (count < N) { k--; }
  // else if (count >= 2*N) { score++; speeder++; linia.play(); }

  else
    { multi++; speeder++;}

 }
  switch(multi)
  {
case 4:
  score += 40;
case 3:
  score +=30;
case 2:
  score += 20;
case 1:
  score += 10;
  linia.play();
  multi = 0;
  break;
default:
    break;
  }
    dx = 0; rotated = 0;
    if (speeder >= 10 && delay >= 0.1f)
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
  s.setTextureRect(IntRect(colorNum[0]*18, 0,18,18));
  s.setPosition(a[i].x*18,a[i].y*18);
  s.move(28,31); //offset
  window.draw(s);

  s.setTextureRect(IntRect(colorNum[1]*18,0,18,18));
  s.setPosition(c[i].x*18+170,c[i].y*18+130);
  s.move(28,31); //offset
  window.draw(s);
   }
 window.draw(frame);
 window.draw(menubg);
 ss.str(std::string());
 ss << std::setfill('0') << std::setw(6) << score;
 txtScore.setString(ss.str());
 window.draw(txtScore);
 window.draw(txtHScore);
 if (fail == 1) GameOver(window, muzyczka, txtGameOver, gameoverbg);
 window.display();
 }
    return 0;
}

//////////////////////////////////////
void GameOver(RenderWindow& window, Music& m, Text& t, RectangleShape& gameoverbg)
{
    if (check())
    {
    m.stop();
    window.draw(gameoverbg);
    window.draw(t);
    window.display();
    cout << score << endl;
    cout << hscoreSource.tellg() << " " << hscoreSource.tellp() << endl;
    sleep(seconds(3));
    }
    if (highScore < score)
     hscoreSource << score;
    window.close();
}

////////////////////////////////////////

template <typename T, size_t N>
void Init(T (&n)[N])
{
    // backgrounds
 menubg.setFillColor(Color(0,0,0, 100));
 menubg.setPosition(240, 30);
 menubg.setSize(nowy);
 gameoverbg.setFillColor(Color(0,0,0,150));
 gameoverbg.setPosition(10,200);
 gameoverbg.setSize(Vector2f(300, 100));
 t1.loadFromFile("images/tiles.png");
 try {if (!t2.loadFromFile("images/background.png")) throw EXIT_FAILURE;}
 catch(int& exc) { cout << "Blad ladowania tla " << exc << endl;}
 t3.loadFromFile("images/frame.png");

    // fonts
 try { if (!czciona.loadFromFile("images/BGOTHL.TTF")) throw EXIT_FAILURE;}
 catch(int&) {cout << "Blad ladowania czcionki." << endl; }
 hscoreSource.open("hscore/hscore.txt", ios::in | ios::out);
 if (!hscoreSource.good()) cout << "nie udalo sie wczytac pliku" << endl;
 hscoreSource >> highScore;
 hscoreSource.clear();
 hscoreSource.seekp(0, ios::beg);
 // hscoreSource.seekp(0);
 txtScore.setFont(czciona);
 txtScore.setScale(0.7f, 0.7f);
 txtScore.setPosition(245, 35);
 txtHScore.setString(ss2.str());
 txtHScore.setFont(czciona);
 txtHScore.setScale(0.7f, 0.7f);
 txtHScore.setPosition(245, 95);
 txtGameOver.setString("Przegrana!\n Pusia wygrywa!");
 txtGameOver.setFont(czciona);
 txtGameOver.setPosition(20,210);
 txtScore.setString(ss.str());
 ss.str(std::string());
 ss2.str(std::string());
 ss2 << std::setfill('0') << std::setw(6) << highScore;
 txtHScore.setString(ss2.str());

    // sounds
 muzyczka.setLoop(1);
 muzyczka.setVolume(5);
 try {if (!bufor.loadFromFile("sounds/line.wav")) throw EXIT_FAILURE;}
 catch(int) {cout << "Blad ladowania dzwieku linii" << endl;}
 try {if (!muzyczka.openFromFile("sounds/muzyczka.ogg")) throw EXIT_FAILURE;}
 catch(int) {cout << "Blad ladowania muzyki." << endl;}
 muzyczka.stop();
 colorNum[1] = 1 + rand() % 7;
    n[0] = n[1];
    n[1] = rand() % 7;
     for (int i = 0; i<4; i++)
    {
     // 3, 5, 4, 7, // T
     a[i].x = (figures[n[0]][i] % 2)+3;
     a[i].y = figures[n[0]][i] / 2;
     c[i].x = (figures[n[1]][i] % 2)+3;
     c[i].y = figures[n[1]][i] / 2;
     }
 //string punkty;
 //hscoreSource << "GOWNO NIE UMIEM" << endl;
 //hscoreSource >> punkty;
 //cout << hscoreSource << endl;
}
    // check bounds
bool check()
{
   for (int i=0;i<4;i++)
   if (a[i].x<0 || a[i].x>=N || a[i].y>=M || field[a[i].y][a[i].x])
        return 1;
   return 0;
};

    // key input
void KeyInput(Event& e)
{
    while (window.pollEvent(e))
    {
        if (e.type == Event::Closed)
            fail = true;
        if (e.type == Event::KeyPressed)
        {
            if (e.key.code==Keyboard::W) rotated=true;
            else if (e.key.code==Keyboard::A) dx=-1;
            else if (e.key.code==Keyboard::D) dx=1;
            else if (e.key.code == Keyboard::E) {score += 10; speeder++;}
            else if (e.key.code == Keyboard::P) paused = !paused;
            else if (e.key.code == Keyboard::T) only_I == true ? only_I = 0: only_I = 1;
            else if (e.key.code == Keyboard::Escape) fail = true;
            else if (e.key.code == Keyboard::M)
            {
                if (!music_on) muzyczka.play();
                else muzyczka.stop();
                music_on = !music_on;
            }
        }
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {delay2 = delay; delay=0.05; }
    else delay = delay2;
}


    // progress
void Update()
{
  timed = sfclock.getElapsedTime().asSeconds();
  sfclock.restart();
  timer+=timed;
    // every tick
  if (timer>delay && !paused)
  {
   for (int i = 0; i<4; i++) { b[i] = a[i]; a[i].y += 1; }
   if (check())
   {
    for (int i = 0; i<4; i++) field[b[i].y][b[i].x] = colorNum[0];
    colorNum[0] = colorNum[1];
    colorNum[1] = 1 + rand() % 7;
        n[0] = n[1];
        if (only_I)
        n[1] = 0;
        else
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

   }
   timer = 0;
  }




}
