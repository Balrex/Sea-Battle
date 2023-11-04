#include <stdio.h>
#include <Windows.h>

#include <conio.h>
#include <locale.h>

# include <stdlib.h>
# include <time.h>

int corably(int razm);

int Prov_Pole_K(int Ki_x, int Ki_y, int Kii_x, int Kii_y);
int napr(int c);


int Pole_Igrok[10][10];                       //массив поля игрока
int Pole_Kom[10][10];                        //массив поля ПК
int Kor_Igrok[5] = { 0, 4, 6, 6, 4};        //оставшиеся клетки не подбитых кораблей [1] однопалубные, [2] двупалубные ....
int Kor_Kom[33];                           //оставшиеся клетки не подбитых кораблей [1] однопалубные, [2] двупалубные ....
int b, a, mn, mk, nn, nk;                 // координаты начала и конца каждого корабля на поле
int i, j, c, d;
int X_min, X_max, Y_min, Y_max;         //координаты поля для выстрела ПК
int X_min2, X_max2, Y_min2, Y_max2;    //координаты поля для выстрела ПК после 1 попадания
int V_pp = 0;                         //попадание в корабль

int Sl_Igr = 0;                      //сложность игры 0-сложная, 1 менее


enum ConsoleColor {
	Black = 0,
	Blue = 1,
	Green = 2,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
};

int corably(int N_razm)   // программа расстановки корабля размерностью razm
{
	int mnapr[5];     // mnapr - массив напрвалений 1-в лево, 2-вправо, 3-вверх, 4-вниз
	int napr;        // количество допустимых напрваление
	int razm;       // размерность корябля 
	               //выбираем разменость коробля из его номера
	switch (N_razm) {
	case 1: razm = 1; break;
	case 4: razm = 4; break;
	case 21: razm = 2; break;
	case 22: razm = 2; break;
	case 23: razm = 2; break;
	case 31: razm = 3; break;
	case 32: razm = 3; break;
	}
		
	do
	{
		for (b = 0; b < razm; ++b)
			mnapr[b] = 0;
		napr = 0;
		while (napr == 0) {
			mn = rand() % 10;              //координата x
			nn = rand() % 10;             // координата У
			if ((mn - (razm - 1)) >= 0) { // выбор направления постановки корабля относительно случано выбранной точки на поле
				++napr;
				mnapr[napr] = 1;
			}
			if ((mn + (razm - 1)) <= 9) {
				++napr;
				mnapr[napr] = 2;
			}
			if ((nn + (razm - 1)) <= 9) {
				++napr;
				mnapr[napr] = 4;
			}
			if ((nn - (razm - 1)) >= 0) {
				++napr;
				mnapr[napr] = 3;
			}
		}

		a = 1 + rand() %(napr);                 // выбор случайного направления

		                                    //расчет координат по случайному направлению 
		switch (mnapr[a]) {
		  case 1: mk = mn - (razm - 1); nk = nn; break;
		  case 2: mk = mn + (razm - 1); nk = nn; break;
		  case 3: mk = mn; nk = nn - (razm - 1); break;
		  case 4: mk = mn; nk = nn + (razm - 1); break;
		}
				
	} while (Prov_Pole_K(mn, nn, mk, nk) != 0); // проверка возможности расположения корабля на случайно выбаранном месте по правилам

	COORD position;										// Объявление необходимой структуры
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода
	
	Pole_Kom[nn][mn] = N_razm;// устоновка корабля на поле
//	Tochka_v_pole(mn, nn, razm + '0', 33, 1, 15, 0, 2);

	Pole_Kom[nk][mk] = N_razm;
//	Tochka_v_pole(mk, nk, razm + '0', 33, 1, 15, 0, 2);
	
	if (razm > 2) {
		if (mn == mk) {
			Pole_Kom[min(nn, nk) + 1][mn] = N_razm;
//			Tochka_v_pole(mn, min(nn, nk) + 1, razm + '0', 33, 1, 15, 0, 2);
			Pole_Kom[min(nn, nk) + 2][mn] = N_razm;
//			Tochka_v_pole(mn, min(nn, nk) + 2, razm + '0', 33, 1, 15, 0, 2);
		}
		else if (nn == nk) {
			Pole_Kom[nn][min(mn, mk)+1] = N_razm;
//			Tochka_v_pole(min(mn, mk)+1, nn, razm + '0', 33, 1, 15, 0, 2);
			Pole_Kom[nn][min(mn, mk)+2] = N_razm;
//			Tochka_v_pole(min(mn, mk)+2, nn, razm + '0', 33, 1, 15, 0, 2);
		}
	}

	return 0;
}

  int Ini_pole (int Kor_X, int Kor_Y) //инициализация поля 
	  //Kor_X, Kor_Y начальные координаты поля Х, У
	  
  {
	  int k,K_x,N_St; 
	char Zag[] = " A B C D E F G H I J ";              //заголовок по Х
	char Pr []= "0" ;
	
	COORD position;										// Объявление необходимой структуры
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода

	SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));

	position.X = Kor_X+3;
	position.Y = Kor_Y;
	SetConsoleCursorPosition(hConsole, position);
	puts(Zag);                                       //подпись оси Х

	position.X = Kor_X+2;								    	// координата X левое вехнее
	position.Y = Kor_Y+1;							   		   // координата Y левое верхнее
	SetConsoleCursorPosition(hConsole, position);      	      // Перемещение каретки по заданным координатам
	puts("+");                                   	         // Вывод угла

	position.X = Kor_X + 24;	                              // координата X правое вехнее
	position.Y = Kor_Y + 1;	                                 // координата Y правое верхнее
	SetConsoleCursorPosition(hConsole, position);           // Перемещение каретки по заданным координатам(hConsole, position);
		puts("+");				                           // Вывод угла
	  
	K_x = Kor_X+3;
	N_St = 0;                                             // стартовая позиция подписи оси Х
	
	for (k = Kor_Y+2; k < Kor_Y+12; ++k) {
		
		position.X = Kor_X;                               //подпись оси У  
		position.Y = k;
		SetConsoleCursorPosition(hConsole, position);
		if (k < Kor_Y + 12) {
			printf(" %d", N_St);
		}
		else
		{
			printf("%d", N_St);
		}
		++N_St;

		position.X = K_x;								  //горизонтальная линия верх  
		position.Y = Kor_Y+1;
		SetConsoleCursorPosition(hConsole, position);  	 
		if (k < Kor_Y + 11) 
		{
			puts("--"); 
		}
		else
		{
			puts("---");
		}

		position.X = K_x;                                //горизонтальная линия низ  
		position.Y = Kor_Y + 12;
		SetConsoleCursorPosition(hConsole, position);
		if (k < Kor_Y + 11)
		{
			puts("--");
		}
		else
		{
			puts("---");
		}
		K_x = K_x + 2;

		position.X = Kor_X + 2;                           //вертикальная линия лево      
		position.Y = k;
		SetConsoleCursorPosition(hConsole, position);
		puts("|");

		position.X = Kor_X + 24;                         //вертикальная линия право    
		position.Y = k;
		SetConsoleCursorPosition(hConsole, position);
		puts("|");
	}

	position.X = Kor_X + 2;                               
	position.Y = Kor_Y + 12;
	SetConsoleCursorPosition(hConsole, position);
	puts("+");                                            // Вывод угла

	position.X = Kor_X + 24;
	position.Y = Kor_Y + 12;
	SetConsoleCursorPosition(hConsole, position);
	puts("+");                                            // Вывод угла
	return 0;    
}

  int Tochka_v_pole(int Kor_x, int Kor_y, char Ss, int Nk_x, int Nk_y, int Fon_col, int Buk_col, int T_goriz) //подпрограмма вывода информации на консоль
	  //Kor_x,Kor_y - координаты из массива, 
	  //Ss - выводимый по координатам символ, 
	  //Nk_x, Nk_у - координаты начальной точки консоли, 
	  //Fon_col - цвет фона по координатам вывода, 
	  //Buk_col - цвет выводимого символа, 
	  //T_goriz = 0 заполнение фоном позиции по координатам, =2  заполнение фоном позиции по координатам +1 поле по горизонтали
  {
	  int K_x[10] = { Nk_x + 4, Nk_x + 6, Nk_x + 8, Nk_x + 10, Nk_x + 12, Nk_x + 14, Nk_x + 16, Nk_x + 18, Nk_x + 20, Nk_x + 22 }; //массив экранных позиций по Х
	  int K_y[10] = { Nk_y + 2, Nk_y + 3, Nk_y + 4, Nk_y + 5, Nk_y + 6, Nk_y + 7, Nk_y + 8, Nk_y + 9, Nk_y + 10, Nk_y + 11 };      //массив экранных позиций по У



	  COORD position;										// Объявление необходимой структуры
	  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода

	  //Цвет символов Buk_col, Цвет фона - Fon_col
	  SetConsoleTextAttribute(hConsole, (WORD)((Fon_col << 4) | Buk_col));

	  if (T_goriz==2)
	  {
		  position.X = K_x[Kor_x] + 1;
		  position.Y = K_y[Kor_y];
		  SetConsoleCursorPosition(hConsole, position);
		  printf(" ");
	  }
	  
	  position.X = K_x[Kor_x];
	  position.Y = K_y[Kor_y];
	  SetConsoleCursorPosition(hConsole, position);
	  printf("%c", Ss);


	  SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
	  position.X = Kor_x;
	  position.Y = Kor_y + 13;
	  SetConsoleCursorPosition(hConsole, position);

	  return 0;
  }

  int Prov_Pole(int Ki_x, int Ki_y, int Kii_x, int Kii_y) //подпрограмма проверки поля вкруг корабля на зянятость
	  //Ki_x, Kii_x первая и вторая координата по х
	  //Ki_y, Kii_y первая и вторая координата по y
	  // если функция вернула 0 то все в порядке, другое число не возможноная комбинация
  {
  	  int i, j, Rez;
	  int N_i, N_j, O_i, O_j;

	  Rez = 0;
		  N_i = min(Ki_x, Kii_x);
	      N_j = min(Ki_y, Kii_y);
		  O_i = max(Ki_x, Kii_x);
		  O_j = max(Ki_y, Kii_y);

		  if (N_i-1 < 0) {N_i = 0;}
		  else { --N_i; }
	      if (N_j - 1 < 0) { N_j = 0; }
	      else { --N_j; }

		  if (O_i + 1 > 9) { O_i = 9; }
		  else { ++O_i; }
		  if (O_j + 1 > 9) { O_j = 9; }
		  else { ++O_j; }

		  for (i = N_i; i < O_i+1; i++)
		  {
			  for (j = N_j; j < O_j + 1; j++) {
				  if (Pole_Igrok[i][j] != 0) {
					  Rez = 1;
					  break;
				  }
		  }
		  }

	  return Rez;
  }

  int Prov_Pole_K(int Ki_x, int Ki_y, int Kii_x, int Kii_y) //подпрограмма проверки поля вкруг корабля на зянятость
	  //Ki_x, Kii_x первая и вторая координата по х
	  //Ki_y, Kii_y первая и вторая координата по y
	  // если функция вернула 0 то все в порядке, другое число не возможноная комбинация
  {
	  int i, j, Rez;
	  int N_i, N_j, O_i, O_j;

	  Rez = 0;
	  N_j = min(Ki_x, Kii_x);
	  N_i = min(Ki_y, Kii_y);
	  O_j = max(Ki_x, Kii_x);
	  O_i = max(Ki_y, Kii_y);

	  if (N_i - 1 < 0) { N_i = 0; }
	  else { --N_i; }
	  if (N_j - 1 < 0) { N_j = 0; }
	  else { --N_j; }

	  if (O_i + 1 > 9) { O_i = 9; }
	  else { ++O_i; }
	  if (O_j + 1 > 9) { O_j = 9; }
	  else { ++O_j; }

	  for (i = N_i; i < O_i + 1; i++)
	  {
		  for (j = N_j; j < O_j + 1; j++) {
			  if (Pole_Kom[i][j] != 0) {
				  Rez = 1;
				  break;
			  }
		  }
	  }

	  return Rez;
  }

  int Zap_Korabl(int N_Pozx, int N_Pozy) // подпрограмма размещения кораблей игрока
  
  {
	  int MVod[2][2];   // массив вводимых значений
	  int NVod = 1;     //направление ввода 1 вставить, -1 удалить    
      int Pal[4] = {4, 3, 2, 1};    // счетчики введеных кораблей
//	  int Pal[4] = { 0, 0, 0, 1 };   //для отладки
	  char VKor[10];    // строка ввода
	  int Kr;          // счетчик цикла
	  int Ssv = 0;     //палуб коробля
	  int Mx, My, Mx1, My1; //для расчета координат 

	  COORD position;										// Объявление необходимой структуры
	  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода
	  position.X = N_Pozx;
	  position.Y = N_Pozy+12;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("Расстановка кораблей игрока");
Loop2:
	  position.X = N_Pozx;                                 //вывод колиечества не расставленых кораблей
	  position.Y = N_Pozy + 13;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("Расставьте: четырех палубный - %d", Pal[3]);

	  position.X = N_Pozx + 15;
	  position.Y = N_Pozy + 14;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("трех палубный - %d", Pal[2]);

	  position.X = N_Pozx + 15;
	  position.Y = N_Pozy + 15;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("двух палубный - %d", Pal[1]);

	  position.X = N_Pozx + 15;
	  position.Y = N_Pozy + 16;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("одно палубный - %d", Pal[0]);

	  
  Loop1:                                                        // ввод координат  
	      NVod = 1;
		  position.X = N_Pozx;
		  position.Y = N_Pozy + 18;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Введите координаты коробля (например: установить корабль A1-A4, удалить корабль /A1-A4):           ");

		  position.X = N_Pozx + 89;
		  position.Y = N_Pozy + 18;
		  SetConsoleCursorPosition(hConsole, position);
		  fflush(stdin);         // очищаем поток ввода
		  fgets(VKor, 8, stdin);

		  position.X = N_Pozx;
		  position.Y = N_Pozy + 19;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("                                                                                                  ");

		  Kr = 0;
		  NVod = 1;
		  //определение ввод 1, удаление -1
		  if (VKor[Kr] == '/') {
			  NVod = -1;
			  Kr = 1;
		     }
			  if (VKor[Kr] == 'A' || VKor[Kr] == 'a') { MVod[0][0] = 0; }                                          //перевод координат в цифру и их проверка
			  else if (VKor[Kr] == 'B' || VKor[Kr] == 'b') { MVod[0][0] = 1; }
			  else if (VKor[Kr] == 'C' || VKor[Kr] == 'c') { MVod[0][0] = 2; }
			  else if (VKor[Kr] == 'D' || VKor[Kr] == 'd') { MVod[0][0] = 3; }
			  else if (VKor[Kr] == 'E' || VKor[Kr] == 'e') { MVod[0][0] = 4; }
			  else if (VKor[Kr] == 'F' || VKor[Kr] == 'f') { MVod[0][0] = 5; }
			  else if (VKor[Kr] == 'G' || VKor[Kr] == 'g') { MVod[0][0] = 6; }
			  else if (VKor[Kr] == 'H' || VKor[Kr] == 'h') { MVod[0][0] = 7; }
			  else if (VKor[Kr] == 'I' || VKor[Kr] == 'i') { MVod[0][0] = 8; }
			  else if (VKor[Kr] == 'J' || VKor[Kr] == 'j') { MVod[0][0] = 9; }
			  else {
				  position.X = N_Pozx;
				  position.Y = N_Pozy + 19;
				  SetConsoleCursorPosition(hConsole, position);
				  printf("Ошибка! Необходимо водить латинские буквы A-J или a-j       ");  goto Loop1;
			      }
			  ++Kr;
			  
			  if (VKor[Kr] == '0') { MVod[0][1] = 0;}
			  else if (VKor[Kr] == '1') { MVod[0][1] = 1; }
			  else if (VKor[Kr] == '2') { MVod[0][1] = 2; }
			  else if (VKor[Kr] == '3') { MVod[0][1] = 3; }
			  else if (VKor[Kr] == '4') { MVod[0][1] = 4; }
			  else if (VKor[Kr] == '5') { MVod[0][1] = 5; }
			  else if (VKor[Kr] == '6') { MVod[0][1] = 6; }
			  else if (VKor[Kr] == '7') { MVod[0][1] = 7; }
			  else if (VKor[Kr] == '8') { MVod[0][1] = 8; }
			  else if (VKor[Kr] == '9') { MVod[0][1] = 9; }
			  else {
				  position.X = N_Pozx;
				  position.Y = N_Pozy + 19;
				  SetConsoleCursorPosition(hConsole, position);
				  printf("Ошибка! Необходимо водить цифры 0-9.");  goto Loop1;
			  }
			  if (VKor[Kr + 1] == 0x0D || VKor[Kr + 1] == 0x0A) {
				  MVod[1][0] = MVod[0][0];
				  MVod[1][1] = MVod[0][1];
				  goto Loop3;
			  }
			  else if (VKor[Kr + 1] != '-') {
					  position.X = N_Pozx;
					  position.Y = N_Pozy + 19;
					  SetConsoleCursorPosition(hConsole, position);
					  printf("Ошибка! Необходимо водить цифры 0-9.");  goto Loop1;
			  }
				  
			  Kr = Kr + 2;
			  if (VKor[Kr] == 'A' || VKor[Kr] == 'a') { MVod[1][0] = 0; }
			  else if (VKor[Kr] == 'B' || VKor[Kr] == 'b') { MVod[1][0] = 1; }
			  else if (VKor[Kr] == 'C' || VKor[Kr] == 'c') { MVod[1][0] = 2; }
			  else if (VKor[Kr] == 'D' || VKor[Kr] == 'd') { MVod[1][0] = 3; }
			  else if (VKor[Kr] == 'E' || VKor[Kr] == 'e') { MVod[1][0] = 4; }
			  else if (VKor[Kr] == 'F' || VKor[Kr] == 'f') { MVod[1][0] = 5; }
			  else if (VKor[Kr] == 'G' || VKor[Kr] == 'g') { MVod[1][0] = 6; }
			  else if (VKor[Kr] == 'H' || VKor[Kr] == 'h') { MVod[1][0] = 7; }
			  else if (VKor[Kr] == 'I' || VKor[Kr] == 'i') { MVod[1][0] = 8; }
			  else if (VKor[Kr] == 'J' || VKor[Kr] == 'j') { MVod[1][0] = 9; }
			  else {
				  position.X = N_Pozx;
				  position.Y = N_Pozy + 19;
				  SetConsoleCursorPosition(hConsole, position);
				  printf("Ошибка! Необходимо водить латинские буквы A-J или a-j       ");  goto Loop1;
			  }
			  ++Kr;
			  if (VKor[Kr+1] == '-') {
				  position.X = N_Pozx;
				  position.Y = N_Pozy + 19;
				  SetConsoleCursorPosition(hConsole, position);
				  printf("Ошибка! Необходимо водить цифры 0-9.");  goto Loop1;
			  }
			  else if (VKor[Kr] == '0') { MVod[1][1] = 0; }
			  else if (VKor[Kr] == '1') { MVod[1][1] = 1; }
			  else if (VKor[Kr] == '2') { MVod[1][1] = 2; }
			  else if (VKor[Kr] == '3') { MVod[1][1] = 3; }
			  else if (VKor[Kr] == '4') { MVod[1][1] = 4; }
			  else if (VKor[Kr] == '5') { MVod[1][1] = 5; }
			  else if (VKor[Kr] == '6') { MVod[1][1] = 6; }
			  else if (VKor[Kr] == '7') { MVod[1][1] = 7; }
			  else if (VKor[Kr] == '8') { MVod[1][1] = 8; }
			  else if (VKor[Kr] == '9') { MVod[1][1] = 9; }
			  else {
				  position.X = N_Pozx;
				  position.Y = N_Pozy + 19;
				  SetConsoleCursorPosition(hConsole, position);
				  printf("Ошибка! Необходимо водить цифры 0-9.");  goto Loop1;
			  }
			  if (MVod[0][0] != MVod[1][0] && MVod[0][1] != MVod[1][1]) {
				  position.X = N_Pozx;
				  position.Y = N_Pozy + 19;
				  SetConsoleCursorPosition(hConsole, position);
				  printf("Введены координаты коробля под углом. Не допустимо. Повторите ввод.");  goto Loop1;
	          }
Loop3:
			  Ssv = abs(MVod[1][0] - MVod[0][0] + MVod[1][1] - MVod[0][1])+1;  //подсчет палубности корабля по координатам
			  switch (Ssv) {                                                   // отражение/удаление корабля в массиве и на консоли
			  case 1:                                                         
				  if (NVod == -1) {
					  if (Pole_Igrok[MVod[0][1]][MVod[0][0]] == 1) {
						  Pal[0] = Pal[0] + 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 0;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  goto Loop2;
					  }
					  else
					  {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Однопалубный корабль не найден. Уточните координаты корабля.");  goto Loop1;
					  }
				  }
				  else {
					  if (Pal[0] - 1 < 0) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Однопалубных кораблей достаточно. Вводите другие типы кораблей.");  goto Loop1;
					  }
					  else if (Prov_Pole(MVod[0][1], MVod[0][0], MVod[1][1], MVod[1][0])) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Нарушается правило размещения кораблей. Измените размещение корабля.");  goto Loop1;
					  }
					  else {
						  Pal[0] = Pal[0] - 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 1;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], '1', N_Pozy, N_Pozx, 15, 0, 2);
					  }
					  break;
				  }
			  case 2:
				  if (NVod == -1) {
					  if (Pole_Igrok[MVod[0][1]][MVod[0][0]] == 2 && Pole_Igrok[MVod[1][1]][MVod[1][0]] == 2) {
						  Pal[1] = Pal[1] + 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 0;
						  Pole_Igrok[MVod[1][1]][MVod[1][0]] = 0;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  Tochka_v_pole(MVod[1][0], MVod[1][1], ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  goto Loop2;
					  }
					  else
					  {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Двухпалубный корабль не найден. Уточните координаты корабля.");  goto Loop1;
					  }
				  }
				  else {
					  if (Pal[1] - 1 < 0) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Двухпалубных кораблей достаточно. Вводите другие типы кораблей.");  goto Loop1;
					  }
					  else if (Prov_Pole(MVod[0][1], MVod[0][0], MVod[1][1], MVod[1][0])) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Нарушается правило размещения кораблей. Измените размещение корабля.");  goto Loop1;
					  }
					  else {
						  Pal[1] = Pal[1] - 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 2;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], '2', N_Pozy, N_Pozx, 15, 0, 2);
						  Pole_Igrok[MVod[1][1]][MVod[1][0]] = 2;
						  Tochka_v_pole(MVod[1][0], MVod[1][1], '2', N_Pozy, N_Pozx, 15, 0, 2);
					  }
					  break;
				  }
			  case 3:
				  if (NVod == -1) {
					  if (MVod[0][0] == MVod[1][0]) {
						  Mx = MVod[0][0];
						  My = min(MVod[0][1], MVod[1][1]) + 1;
					  }
					  else {
						  Mx = min(MVod[0][0], MVod[1][0]) + 1;
						  My = MVod[1][1];
					  }
					  if (Pole_Igrok[MVod[0][1]][MVod[0][0]] == 3 && Pole_Igrok[MVod[1][1]][MVod[1][0]] == 3 && Pole_Igrok[My][Mx] == 3) {
						  Pal[2] = Pal[2] + 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 0;
						  Pole_Igrok[MVod[1][1]][MVod[1][0]] = 0;
						  Pole_Igrok[My][Mx] = 0;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  Tochka_v_pole(MVod[1][0], MVod[1][1], ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  Tochka_v_pole(Mx, My, ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  goto Loop2;
					  }
					  else
					  {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Трехпалубный корабль не найден. Уточните координаты корабля.");  goto Loop1;
					  }
				  }
				  else {
					  if (Pal[2] - 1 < 0) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Трехпалубных кораблей достаточно. Вводите другие типы кораблей.");  goto Loop1;
					  }
					  else if (Prov_Pole(MVod[0][1], MVod[0][0], MVod[1][1], MVod[1][0])) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Нарушается правило размещения кораблей. Измените размещение корабля.");  goto Loop1;
					  }
					  else {
						  Pal[2] = Pal[2] - 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 3;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], '3', N_Pozy, N_Pozx, 15, 0, 2);
						  Pole_Igrok[MVod[1][1]][MVod[1][0]] = 3;
						  Tochka_v_pole(MVod[1][0], MVod[1][1], '3', N_Pozy, N_Pozx, 15, 0, 2);
						  if (MVod[0][0] == MVod[1][0]) {
							  Pole_Igrok[min(MVod[0][1], MVod[1][1]) + 1][MVod[0][0]] = 3;
							  Tochka_v_pole(MVod[0][0], min(MVod[0][1], MVod[1][1]) + 1, '3', N_Pozy, N_Pozx, 15, 0, 2);
						  }
						  else {
							  Pole_Igrok[MVod[1][1]][min(MVod[0][0], MVod[1][0]) + 1] = 3;
							  Tochka_v_pole(min(MVod[0][0], MVod[1][0]) + 1, MVod[1][1], '3', N_Pozy, N_Pozx, 15, 0, 2);
						  }
					  }
					  break;
				  }
			  case 4:
				  if (NVod == -1) {
					  if (MVod[0][0] == MVod[1][0]) {
						  Mx = MVod[0][0];
						  My = min(MVod[0][1], MVod[1][1]) + 1;
						  Mx1 = Mx;
						  My1 = My + 1;
					  }
					  else {
						  Mx = min(MVod[0][0], MVod[1][0]) + 1;
						  My = MVod[1][1];
						  Mx1 = Mx + 1;
						  My1 = My;
					  }
					  if (Pole_Igrok[MVod[0][1]][MVod[0][0]] == 4 && Pole_Igrok[MVod[1][1]][MVod[1][0]] == 4 && Pole_Igrok[My][Mx] == 4 && Pole_Igrok[My1][Mx1] == 4) {
						  Pal[3] = Pal[3] + 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 0;
						  Pole_Igrok[MVod[1][1]][MVod[1][0]] = 0;
						  Pole_Igrok[My][Mx] = 0;
						  Pole_Igrok[My1][Mx1] = 0;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  Tochka_v_pole(MVod[1][0], MVod[1][1], ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  Tochka_v_pole(Mx, My, ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  Tochka_v_pole(Mx1, My1, ' ', N_Pozy, N_Pozx, 0, 15, 2);
						  goto Loop2;
					  }
					  else
					  {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Четырехпалубный корабль не найден. Уточните координаты корабля.");  goto Loop1;
					  }
				  }
				  else {
					  if (Pal[3] - 1 < 0) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Четырехпалубный корабль установлен. Вводите другие типы кораблей.");  goto Loop1;
					  }
					  else if (Prov_Pole(MVod[0][1], MVod[0][0], MVod[1][1], MVod[1][0])) {
						  position.X = N_Pozx;
						  position.Y = N_Pozy + 19;
						  SetConsoleCursorPosition(hConsole, position);
						  printf("Нарушается правило размещения кораблей. Измените размещение корабля.");  goto Loop1;
					  }
					  else {
						  Pal[3] = Pal[3] - 1;
						  Pole_Igrok[MVod[0][1]][MVod[0][0]] = 4;
						  Tochka_v_pole(MVod[0][0], MVod[0][1], '4', N_Pozy, N_Pozx, 15, 0, 2);
						  Pole_Igrok[MVod[1][1]][MVod[1][0]] = 4;
						  Tochka_v_pole(MVod[1][0], MVod[1][1], '4', N_Pozy, N_Pozx, 15, 0, 2);
						  if (MVod[0][0] == MVod[1][0]) {
							  Pole_Igrok[min(MVod[0][1], MVod[1][1]) + 1][MVod[0][0]] = 4;
							  Tochka_v_pole(MVod[0][0], min(MVod[0][1], MVod[1][1]) + 1, '4', N_Pozy, N_Pozx, 15, 0, 2);
							  Pole_Igrok[min(MVod[0][1], MVod[1][1]) + 2][MVod[0][0]] = 4;
							  Tochka_v_pole(MVod[0][0], min(MVod[0][1], MVod[1][1]) + 2, '4', N_Pozy, N_Pozx, 15, 0, 2);
						  }
						  else {
							  Pole_Igrok[MVod[1][1]][min(MVod[0][0], MVod[1][0]) + 1] = 4;
							  Tochka_v_pole(min(MVod[0][0], MVod[1][0]) + 1, MVod[1][1], '4', N_Pozy, N_Pozx, 15, 0, 2);
							  Pole_Igrok[MVod[1][1]][min(MVod[0][0], MVod[1][0]) + 2] = 4;
							  Tochka_v_pole(min(MVod[0][0], MVod[1][0]) + 2, MVod[1][1], '4', N_Pozy, N_Pozx, 15, 0, 2);
						  }
					  }
					  break;
				  }
			  default:
				  position.X = N_Pozx;
				  position.Y = N_Pozy + 19;
				  SetConsoleCursorPosition(hConsole, position);
				  printf("Не верный формат координат корабля! Повторите ввод.");  goto Loop1;
				  break;
			  }



			  position.X = N_Pozx + 99;
			  position.Y = N_Pozy + 18;
			  SetConsoleCursorPosition(hConsole, position);
			    			  	  
	  //printf("%d %d - %d %d %d %d %d Nap-%d", NVod, MVod[0][0], MVod[0][1], MVod[1][0], MVod[1][1], Ssv, Pole_Igrok[MVod[0][0]][MVod[0][1]], NVod);

	  if (Pal[0] + Pal[1] + Pal[2] + Pal[3] != 0) { goto Loop2; }   // все ли корабли расставлены

	  SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
	  position.X = N_Pozx;
	  position.Y = N_Pozy + 20;
	  SetConsoleCursorPosition(hConsole, position);

	  return 0;
  }

  int Hod_Igrok(int Ig_Pozx, int Ig_Pozy) {
	  int Ny, Nx;         //для цикла
	  int EndI;          //конец игры 
	  char VKor[10];    // строка ввода
	  int Kr;          // счетчик цикла
	  int Ssv = 0;    //палуб коробля
	  int Ix, Iy;    //для расчета координат 
	  int X_Vk = 0; // координата х для вывода строк
	    

	  i = 0;
	  COORD position;										// Объявление необходимой структуры
	  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода
		 
Loop10:
	  position.X = X_Vk;
	  position.Y = Ig_Pozy + 14;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("Ваш выстрел. Введите координаты поля компьютера (например: A1 или a1):                             ");

	  position.X = X_Vk + 71;
	  position.Y = Ig_Pozy + 14;
	  SetConsoleCursorPosition(hConsole, position);
	  fflush(stdin);         // очищаем поток ввода
	  fgets(VKor, 8, stdin);

	  position.X = X_Vk;
	  position.Y = Ig_Pozy + 17;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                             ");

	  position.X = X_Vk;
	  position.Y = Ig_Pozy + 18;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                             ");

	  Kr = 0;
	 
	  if (VKor[Kr] == 'A' || VKor[Kr] == 'a') { Ix = 0; }                                          //перевод координат в цифру и их проверка
	  else if (VKor[Kr] == 'B' || VKor[Kr] == 'b') { Ix = 1; }
	  else if (VKor[Kr] == 'C' || VKor[Kr] == 'c') { Ix = 2; }
	  else if (VKor[Kr] == 'D' || VKor[Kr] == 'd') { Ix = 3; }
	  else if (VKor[Kr] == 'E' || VKor[Kr] == 'e') { Ix = 4; }
	  else if (VKor[Kr] == 'F' || VKor[Kr] == 'f') { Ix = 5; }
	  else if (VKor[Kr] == 'G' || VKor[Kr] == 'g') { Ix = 6; }
	  else if (VKor[Kr] == 'H' || VKor[Kr] == 'h') { Ix = 7; }
	  else if (VKor[Kr] == 'I' || VKor[Kr] == 'i') { Ix = 8; }
	  else if (VKor[Kr] == 'J' || VKor[Kr] == 'j') { Ix = 9; }
	  else {
		  position.X = X_Vk;
		  position.Y = Ig_Pozy + 15;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Ошибка! Необходимо водить латинские буквы A-J или a-j       ");  goto Loop10;
	  }
	  ++Kr;
	  if (VKor[Kr + 1] != 0x0D && VKor[Kr + 1] != 0x0A) {
		  position.X = X_Vk;
		  position.Y = Ig_Pozy + 15;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Ошибка! Необходимо водить цифры 0-9.                        ");  goto Loop10;
	  }
	  if (VKor[Kr] == '0') { Iy = 0; }
	  else if (VKor[Kr] == '1') { Iy = 1; }
	  else if (VKor[Kr] == '2') { Iy = 2; }
	  else if (VKor[Kr] == '3') { Iy = 3; }
	  else if (VKor[Kr] == '4') { Iy = 4; }
	  else if (VKor[Kr] == '5') { Iy = 5; }
	  else if (VKor[Kr] == '6') { Iy = 6; }
	  else if (VKor[Kr] == '7') { Iy = 7; }
	  else if (VKor[Kr] == '8') { Iy = 8; }
	  else if (VKor[Kr] == '9') { Iy = 9; }
	  else {
		  position.X = X_Vk;
		  position.Y = Ig_Pozy + 15;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Ошибка! Необходимо водить цифры 0-9.                        ");  goto Loop10;
	  }

	  // проверка выстрела
	  Ssv = Pole_Kom[Iy][Ix];
	  
	  if (Sl_Igr == 1) {
		  if (Ssv < 0) {
			  position.X = X_Vk;
			  position.Y = Ig_Pozy + 15;
			  SetConsoleCursorPosition(hConsole, position);
			  printf("Вы уже стреляли по этому полю. Буддте внимательнее.       "); goto Loop10;
		  }
	  }

	  if (Ssv > 0) {
		  if (Ssv == 1 || Kor_Kom[Ssv] == 1) {
			  position.X = X_Vk;
			  position.Y = Ig_Pozy + 15;
			  SetConsoleCursorPosition(hConsole, position);
			  printf("Поздравляю! Вы убили корабль.                                                                                    ");
			  
			  Tochka_v_pole(Ix, Iy, ' ', Ig_Pozx, Ig_Pozy, 4, 0, 2);
			  if (Ssv != 1) {
				  for (Ny = 0; Ny < 10; ++Ny) {
					  for (Nx = 0; Nx < 10; ++Nx) {
						  if (Pole_Kom[Ny][Nx] == -Ssv) {
							  Tochka_v_pole(Nx, Ny, ' ', Ig_Pozx, Ig_Pozy, 4, 0, 2);
						  }
					  }
				  }
			  }
		  }
	     else if (Kor_Kom[Ssv] > 1) {
		    position.X = X_Vk;
		    position.Y = Ig_Pozy + 15;
		    SetConsoleCursorPosition(hConsole, position);
		    printf("Метко стреляее! Вы ранили корабль.                                                                               ");
			if (Sl_Igr == 1 && Kor_Kom[Ssv] > 0) Tochka_v_pole(Ix, Iy, ' ', Ig_Pozx, Ig_Pozy, 14, 0, 2);
			else   Tochka_v_pole(Ix, Iy, ' ', Ig_Pozx, Ig_Pozy, 4, 0, 2);
	     }
	     Kor_Kom[Ssv] = Kor_Kom[Ssv] - 1;
	     Pole_Kom[Iy][Ix] = -Ssv;      // поле выстрела попадания
		
		 i = Kor_Kom[1] + Kor_Kom[21] + Kor_Kom[22] + Kor_Kom[23] + Kor_Kom[31] + Kor_Kom[32] + Kor_Kom[4];
		 if (i != 0) goto Loop10;
      }
	  else if (Ssv < 1) {
		  position.X = X_Vk;
		  position.Y = Ig_Pozy + 15;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Мимо! Цельтесь точнее.                                                                                             ");
		  if (Ssv == 0) {
			  Pole_Kom[Iy][Ix] = -7;      // поле выстрела мимо
			  Tochka_v_pole(Ix, Iy, ' ', Ig_Pozx, Ig_Pozy, 8, 0, 2);
		  }
//		  position.X = X_Vk + 55;
//		  position.Y = Ig_Pozy + 15;
//		  SetConsoleCursorPosition(hConsole, position);
//		  fflush(stdin);         // очищаем поток ввода
//		  fgets(VKor, 8, stdin);
	  }

//	  position.X = X_Vk + 80;
//	  position.Y = Ig_Pozy + 15;
//	  SetConsoleCursorPosition(hConsole, position);
//	  printf("%d 1=%d 21=%d 22=%d 23=%d 31=%d 32=%d 4=%d 0=%d ", Ssv, Kor_Kom[1], Kor_Kom[21], Kor_Kom[22], Kor_Kom[23], Kor_Kom[31], Kor_Kom[33], Kor_Kom[4], Kor_Kom[0]);
	  
	  return 0;
  }

  int Hod_Kom(int Ig_Pozx, int Ig_Pozy) {
	  
	  char VKor[10];                                // строка ввода
	  int mnapr[5];                                // mnapr - массив напрвалений 1-в лево, 2-вправо, 3-вверх, 4-вниз
	  int napr;                                   // количество допустимых напрваление
	  int Ssv = 0;                               //палуб коробля
	  char Zag[10] = "ABCDEFGHIJ";              //заголовок по Х
	  int X_Vk = 0;                            // координата х для вывода строк
	  int xmi, xma, ymi, yma, kx, ky;         // счетчики для координат 
	  
	  int Sw;                               // счет rand значений

	  if (V_pp == 0) {
		  X_min = 0;                                                  //при новом выстреле все поле
		  X_max = 9;
		  Y_min = 0;
		  Y_max = 9;
	  }
	  COORD position;								     		// Объявление необходимой структуры
	  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);       // Получение дескриптора устройства стандартного вывода
	  	 
  Loop20:
//	  position.X = X_Vk + 77;
//	  position.Y = Ig_Pozy + 17;
//	  SetConsoleCursorPosition(hConsole, position);
//	  printf("V_pp=%d mn=%d nn=%d X_mi=%d X_ma=%d Y_mi=%d Y_ma=%d", V_pp, mn, nn, X_min, X_max, Y_min, Y_max);


	  position.X = X_Vk;
	  position.Y = Ig_Pozy + 17;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("Мой выстрел, координаты:                                               ");
	  Sw = 0;
	  do {
		  mn = X_min + rand() % (X_max-X_min + 1);                         //координата x
		  nn = Y_min + rand() % (Y_max-Y_min + 1);                        // координата У
		  Sw++;
		  if (Sw > ((X_max - X_min + 1) * (Y_max - Y_min + 1) + 18)) {
			  Sw = 0;
			  if (V_pp == 1) {
				  X_min = X_min2;
				  X_max = X_max2;
				  Y_min = Y_min2;
				  Y_max = Y_max2;
				  V_pp = 0;
			  }
			  else {
				  X_min = 0;                                                  //расширение поля если не нашлись координаты
				  X_max = 9;
				  Y_min = 0;
				  Y_max = 9;
			  }
		  }
	  } while (Pole_Igrok[nn][mn] < 0);           // ищем куда еще не стреляли  
	  
		  VKor[0] = Zag[mn];
		  VKor[1] = nn + '0';
		  position.X = X_Vk + 25;
		  position.Y = Ig_Pozy + 17;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("%c%c ", VKor[0], VKor[1]);
		  
		  Ssv = Pole_Igrok[nn][mn];

//		  position.X = X_Vk + 77;
//		  position.Y = Ig_Pozy + 18;
//		  SetConsoleCursorPosition(hConsole, position);
//		  printf("V_pp=%d mn=%d nn=%d X_mi=%d X_ma=%d Y_mi=%d Y_ma=%d", V_pp, mn, nn, X_min, X_max, Y_min, Y_max);

		  if (Ssv < 1) {		  
			  if (Ssv == 0) {
				  Pole_Igrok[nn][mn] = -7;                                 // поле выстрела мимо
				  Tochka_v_pole(mn, nn, ' ', Ig_Pozx, Ig_Pozy, 8, 0, 2);
			  }
			  position.X = X_Vk;
			  position.Y = Ig_Pozy + 18;
			  SetConsoleCursorPosition(hConsole, position);
			  printf("Какая жалость, я промазал!                                                  ");

//			  position.X = X_Vk + 59;
//			  position.Y = Ig_Pozy + 18;
//			  SetConsoleCursorPosition(hConsole, position);
//			  fflush(stdin);         // очищаем поток ввода
//			  fgets(VKor, 8, stdin);
		  }
		  else if (Ssv > 0) {

			  if (Kor_Igrok[Ssv] > 0) {
				  Pole_Igrok[nn][mn] = -Ssv;                                 // поле выстрела попал
				  Tochka_v_pole(mn, nn, ' ', Ig_Pozx, Ig_Pozy, 4, 0, 2);
				  Kor_Igrok[Ssv] = Kor_Igrok[Ssv] - 1;
				  if (mn - 1 < 0) {
					  xmi = 0;
					  xma = mn + 1;
				  }
				  else if (mn + 1 > 9) {
					  xmi = mn - 1;
					  xma = 9;
				  }
				  else {
					  xmi = mn - 1;
					  xma = mn + 1;
				  }
				  if (nn - 1 < 0) {
					  ymi = 0;
					  yma = nn + 1;
				  }
				  else if (nn + 1 > 9) {
					  ymi = nn - 1;
					  yma = 9;
				  }
				  else {
					  ymi = nn - 1;
					  yma = nn + 1;
				  }

				  if (Ssv == 2 && (Kor_Igrok[Ssv] == 4 || Kor_Igrok[Ssv] == 2 || Kor_Igrok[Ssv] == 0))  V_pp = 0;   //проверка подбит корабль Да то V_pp = 0
				  else if (Ssv == 2 && (Kor_Igrok[Ssv] == 5 || Kor_Igrok[Ssv] == 3 || Kor_Igrok[Ssv] == 1))  V_pp = 1;
				  else if (Ssv == 3 && (Kor_Igrok[Ssv] == 3 || Kor_Igrok[Ssv] == 0))  V_pp = 0;
				  else if (Ssv == 3 && (Kor_Igrok[Ssv] == 5 || Kor_Igrok[Ssv] == 2))  V_pp = 2;
				  else if (Ssv == 3 && (Kor_Igrok[Ssv] == 4 || Kor_Igrok[Ssv] == 1))  V_pp = 1;
				  else if (Ssv == 4 && Kor_Igrok[Ssv] == 0)  V_pp = 0;
				  else if (Ssv == 4 && Kor_Igrok[Ssv] == 3 )  V_pp = 2;
				  else if (Ssv == 4 && (Kor_Igrok[Ssv] == 1 || Kor_Igrok[Ssv] == 2))  V_pp = 1;
				  else if (Ssv == 1) V_pp = 0;

				  if (Sl_Igr == 1) {
					  for (ky = ymi; ky <= yma; ky += 2) {
						  for (kx = xmi; kx <= xma; kx += 2) {
							  if (Pole_Igrok[ky][kx] == 0) Pole_Igrok[ky][kx] = -7;                            // поле вокруг коробля не стрелять
						  }
					  }
				  }
				  else {
					  for (ky = ymi; ky <= yma; ky++) {
						  for (kx = xmi; kx <= xma; kx++) {
							  if (Pole_Igrok[ky][kx] == 0) Pole_Igrok[ky][kx] = -7;                            // поле вокруг коробля не стрелять
						  }
					  }
				  }

				  if (V_pp == 2) {
					  X_min = xmi;
					  X_max = xma;
					  Y_min = ymi;
					  Y_max = yma;
				  }
				  else if (V_pp == 1) {
					  X_min2 = X_min;
					  X_max2 = X_max;
					  Y_min2 = Y_min;
					  Y_max2 = Y_max;

					  X_min = xmi;
					  X_max = xma;
					  Y_min = ymi;
					  Y_max = yma;
				  }
			  }

			  position.X = X_Vk;
			  position.Y = Ig_Pozy + 18;
			  SetConsoleCursorPosition(hConsole, position);
			  printf("Ура, я попал! Для следующего хода нажмите Enter                         ");


			  position.X = X_Vk + 48;
			  position.Y = Ig_Pozy + 18;
			  SetConsoleCursorPosition(hConsole, position);
			  fflush(stdin);         // очищаем поток ввода
			  fgets(VKor, 8, stdin);

			  position.X = X_Vk;
			  position.Y = Ig_Pozy + 14;
			  SetConsoleCursorPosition(hConsole, position);
			  printf("                                                                                                                ");

			  position.X = X_Vk;
			  position.Y = Ig_Pozy + 15;
			  SetConsoleCursorPosition(hConsole, position);
			  printf("                                                                                                                ");
			  if (Kor_Igrok[1] + Kor_Igrok[2] + Kor_Igrok[3] + Kor_Igrok[4] != 0) {
				  goto Loop20;
			  }
		  }
	  
	  return 0;
 }


  int main() {

	  char VKor[10];                        // строка ввода
	  int Npol1_y = 3;                     //начальная точка поля 1 по y
	  int Npol1_x = 1;                    //начальная точка поля 1 по x
	  int Npol2_y = 33;                  //начальная точка поля 2 по y
	  int Npol2_x = 1;                  //начальная точка поля 2 по x
	  int S_korIg = 20;                //количество кораблей игрока
	  int S_korPk = 20;               //количество кораблей компьютера
	 
	 

	  setlocale(LC_ALL, "Rus");


	  Ini_pole(Npol1_y, Npol1_x);       //инициализация поля 1
	  Ini_pole(Npol2_y, Npol2_x);      //инициализация поля 2    
	 

	  Zap_Korabl(Npol1_x, Npol1_y);   //расстановка кораблей

	  srand(time(0));                 // сброс случайного числа
	  corably(4);                    //установка 4-х палубного корабля ПК
	  corably(31);
	  corably(32);
	  corably(21);
	  corably(22);
	  corably(23);
	  corably(1);
	  corably(1);
	  corably(1);
	  corably(1);
	  Kor_Kom[1] = 4;               //инициализация клеток кораблей ПК для подсчета ранненых
	  Kor_Kom[4] = 4;
	  Kor_Kom[21] = 2;
	  Kor_Kom[22] = 2;
	  Kor_Kom[23] = 2;
	  Kor_Kom[31] = 3;
	  Kor_Kom[32] = 3;

//	  printf("\n");
//	  for (i = 0; i < 10; ++i) {
//		  for (j = 0; j < 10; ++j)
//			  printf("%d ", Pole_Igrok[i][j]);
//		  printf("\n");
//	  }

	  COORD position;										// Объявление необходимой структуры
	  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	// Получение дескриптора устройства стандартного вывода

	  //стираем предыдущий вывод
	  position.X = Npol1_x;
	  position.Y = Npol1_y + 13;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 14;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 15;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 16;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 17;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 18;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");
  
  VPovd:
	  position.X = Npol1_x;
	  position.Y = Npol1_y + 13;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("Выберите сложность игры. Не сложно введите 1, сложно введите 0.  По завершении ввода нажмите Enter:                   ");
	  position.X = Npol1_x+ 100;
	  position.Y = Npol1_y + 13;
	  SetConsoleCursorPosition(hConsole, position);
	  fflush(stdin);         // очищаем поток ввода
	  fgets(VKor, 4, stdin);

	  if (VKor[0] == '0') Sl_Igr = 0;
	  else if (VKor[0] == '1')Sl_Igr = 1;
	  else {
		  position.X = Npol1_x;
		  position.Y = Npol1_y + 14;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Не понял Ваш выбор! Повторите еще раз.                                                                      ");
		  goto VPovd;
	  }

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 13;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 14;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 15;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

Loop50:
	  Hod_Igrok(Npol2_y, Npol2_x);
	  Hod_Kom(Npol1_y, Npol1_x);

	  S_korPk = Kor_Kom[1] + Kor_Kom[21] + Kor_Kom[22] + Kor_Kom[23] + Kor_Kom[31] + Kor_Kom[32] + Kor_Kom[4];
	  S_korIg = Kor_Igrok[1] + Kor_Igrok[2] + Kor_Igrok[3] + Kor_Igrok[4];

//	  position.X = Npol1_x;
//	  position.Y = Npol1_y + 19;
//	  SetConsoleCursorPosition(hConsole, position);
//	  for (i = 0; i < 10; ++i) {
//   	  for (j = 0; j < 10; ++j)
//			  printf("%d ", Pole_Igrok[i][j]);
//		  printf("\n");
//	  }
	  
	  if (S_korPk != 0 && S_korIg != 0) goto Loop50;

	  //стираем предыдущий вывод
	  position.X = Npol1_x;
	  position.Y = Npol1_y + 13;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 14;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 15;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 16;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 17;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  position.X = Npol1_x;
	  position.Y = Npol1_y + 18;
	  SetConsoleCursorPosition(hConsole, position);
	  printf("                                                                                                                      ");

	  if (S_korPk == 0) {
		  position.X = Npol1_x;
		  position.Y = Npol1_y + 13;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Досадно! Я проиграл. Спасибо за игру. Выиграю в следующий раз.   Для завершение нажмите Enter:                    ");
	  }
	  if (S_korIg == 0) {
		 
		  for (i = 0; i < 10; ++i) {
			  for (j = 0; j < 10; ++j) {
				  if (Pole_Kom[i][j] > 0) {
					  Tochka_v_pole(j, i, ' ', Npol2_y, Npol2_x, 2, 0, 2);
				  }
			  }
		  }
		  		  
		  position.X = Npol1_x-1;
		  position.Y = Npol1_y + 13;
		  SetConsoleCursorPosition(hConsole, position);
		  printf("Я выиграл. Спасибо за игру. Вы достойный соперник.  Для завершение нажмите Enter:                                   ");

		  fflush(stdin);         // очищаем поток ввода
		  fgets(VKor, 8, stdin);
		  	 
	  }

	  fflush(stdin);         // очищаем поток ввода
	  fgets(VKor, 8, stdin);
	  position.X = Npol1_x;
	  position.Y = Npol1_y + 15;
	  SetConsoleCursorPosition(hConsole, position);

	  return 0;
  }