
#include <SFML/Graphics.hpp>
#include <iostream>

const int SIZE = 100;
sf::Sprite sFigure[32];
sf::Texture Board, Figure, Button;
bool figureMove; //0 bialy 1 czarny
std::string side = "white";

//SZACHOWNICA
int boardWhite[8][8] ={ 
                         5,  4,  3,  2,  1,  3,  4,  5,
                         6,  6,  6,  6,  6,  6,  6,  6,
                         0,  0,  0,  0,  0,  0,  0,  0,
                         0,  0,  0,  0,  0,  0,  0,  0,
                         0,  0,  0,  0,  0,  0,  0,  0,
                         0,  0,  0,  0,  0,  0,  0,  0,
                        -6, -6, -6, -6, -6, -6, -6, -6,
                        -5, -4, -3, -2, -1, -3, -4, -5  
};

int boardBlack[8][8] = { 
                         -5, -4, -3, -1, -2, -3, -4, -5,
                         -6, -6, -6, -6, -6, -6, -6, -6,
                          0,  0,  0,  0,  0,  0,  0,  0,
                          0,  0,  0,  0,  0,  0,  0,  0,
                          0,  0,  0,  0,  0,  0,  0,  0,
                          0,  0,  0,  0,  0,  0,  0,  0,
                          6,  6,  6,  6,  6,  6,  6,  6,
                          5,  4,  3,  1,  2,  3,  4,  5 
};
 
//ZALADOWANIE FIGUR NA SZACHOWNICE Z PLIKU PNG
void loadPosition(std::string side)
{
    int k = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            int n = 0;
            if(side == "white")
                n = boardWhite[i][j];
            else
                n = boardBlack[i][j];
            if (!n) continue;
            int x = abs(n) - 1;
            int y = n > 0 ? 1 : 0;
            sFigure[k].setTextureRect(sf::IntRect(SIZE * x, SIZE * y, SIZE, SIZE));
            sFigure[k].setPosition(SIZE * j, SIZE * i);
            k++;
        }
}

//ZWROCENIE POZYCJI FIGURY
std::string toChessNote(sf::Vector2f p)
{
    std::string s = "";
    s += char(p.x/SIZE+97);
    s += char(7-p.y/SIZE+49);
    return s;
}

//NA WSPOLRZEDNE
sf::Vector2f toCoord(char a, char b)
{
    int x = int(a) - 97;
    int y = 7 - int(b) + 49;
    return sf::Vector2f(x * SIZE, y* SIZE);
}

//RUCH FIGURY
void move(std::string str)
{
    // "a2 a3"
    sf::Vector2f oldPos = toCoord(str[0], str[1]);
    sf::Vector2f newPos = toCoord(str[3], str[4]);

    for (int i = 0; i < 32; i++)
        if (sFigure[i].getPosition() == newPos)
            sFigure[i].setPosition(-100, -100);

    for (int i = 0; i < 32; i++)
        if (sFigure[i].getPosition() == oldPos)
            sFigure[i].setPosition(newPos);
}


int main()
{
    //ZALADOWANIE SZACHOWNICY
    Board.loadFromFile("img/board.png");
    sf::Sprite sBoard(Board);

    /*
    Button.loadFromFile("img/newgame.png");
    sf::Sprite sButton(Button);
    sButton.setTextureRect(sf::IntRect(200, 850, 400, 100));
    sButton.setPosition(200, 850);
    */
    
    float mouseX = 0;
    float mouseY = 0;
    bool isMove = false;
    int n = 0;
    sf::Vector2f oldPosition, newPosition;
    std::string currentPosition;
    std::string previousPosition;

    //ZALADOWANIE FIGUR NA POLACH
    Figure.loadFromFile("img/figure.png");
    for (int i = 0; i < 32; i++)
        sFigure[i].setTexture(Figure);
    //kolor figur gracza
    loadPosition(side);

    //STWORZENIE OKNA
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Game by Filip Kurasz");

    //PROGRAM DZIALA TAK DLUGO JAK OKNO JEST OTWARTE
    while (window.isOpen())
    {
        sf::Event event;
        //POZYCJA MYSZY NA OKNIE
        sf::Vector2i position = sf::Mouse::getPosition(window);
        //SPRAWDZ WSZYSTKIE ZDARZENIA OKNA, KTORE ZOSTALY WYWOLANE OD OSTATNEIJ ITERACJI PETLI
        while (window.pollEvent(event))
        {
            //"ZAMKNIJ ZADANIE" ZDARZENIE: ZAMYKANIE OKNA
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
                //PRZECIAGNIJ I UPUSC
                if (event.type == sf::Event::MouseButtonPressed)
                    if (event.key.code == sf::Mouse::Left)
                        for (int i = 0; i < 32; i++)
                            if (sFigure[i].getGlobalBounds().contains(position.x, position.y))
                            {
                                isMove = true;
                                n = i;
                                mouseX = position.x - sFigure[i].getPosition().x;
                                mouseY = position.y - sFigure[i].getPosition().y;
                                oldPosition = sFigure[i].getPosition();
                            }
                //AUTODOPASOWANIE DO POLA
                if (event.type == sf::Event::MouseButtonReleased)
                    if (event.key.code == sf::Mouse::Left)
                    {
                        isMove = false;
                        sf::Vector2f positionInSquare = sFigure[n].getPosition() + sf::Vector2f(SIZE / 2, SIZE / 2);
                        sf::Vector2f newPosition = sf::Vector2f(SIZE * int(positionInSquare.x / SIZE), SIZE * int(positionInSquare.y / SIZE));

                        //POZYCJA FIGURY W KONSOLI
                        if (toChessNote(oldPosition) != toChessNote(newPosition))
                        {
                            currentPosition = toChessNote(oldPosition) + " " + toChessNote(newPosition);
                        }
                        if (currentPosition != previousPosition)
                        {
                            std::cout << currentPosition << std::endl;
                            previousPosition = currentPosition;
                            move(currentPosition);
                        }
                        sFigure[n].setPosition(newPosition);
                    }
            
        //RUCH KOMPUTERA
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            currentPosition = "d7 d5";

            oldPosition = toCoord(currentPosition[0], currentPosition[1]);
            newPosition = toCoord(currentPosition[3], currentPosition[4]);

            for (int i = 0; i < 32; i++) if (sFigure[i].getPosition() == oldPosition) n = i;

            //ANIMACJA
            sf::Vector2f p = newPosition - oldPosition;
            sFigure[n].move(p.x / 1000, p.y / 1000);
            window.draw(sBoard);
            for (int i = 0; i < 32; i++) 
                window.draw(sFigure[i]); 
            window.draw(sFigure[n]);
            window.display();
            
            if (currentPosition != previousPosition)
                {
                    std::cout << currentPosition << std::endl;
                    previousPosition = currentPosition;
                    move(currentPosition);
                }
            
            sFigure[n].setPosition(newPosition);
        }


        if (isMove) sFigure[n].setPosition(position.x - mouseX, position.y - mouseY);
        

        //WYSWIETL SZACHOWNICE
        window.draw(sBoard);
        for (int i = 0; i < 32; i++)
            window.draw(sFigure[i]);
        //window.draw(sButton);
        window.display();
    }
    return 0;
}
