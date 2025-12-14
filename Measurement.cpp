// ******** Измерение времени падения цепочки ****************** 
// ******** (С) Степан Воротницкий *****************************
// ************************************************** вер. 3 ***

// ******** Библиотеки ***********
#include <eCOM.h> // общие определения констант для учебного стенда
#include <eGFX.h> //TFT дисплей и клавиатура

// ************ Определение глобальных объектов ***************
GFX1 TFT; //TFT дисплей    
volatile bool butPrs = true; //индикатор нажатия кнопки

// ************** Объявления функций *************************
void ButtonPressed();

// ********************** SETUP *****************************
void setup() {   
  pinMode (PIN_BUTTON, INPUT_PULLUP);  //Цифровой пин кнопки 
  pinMode (PIN_EXT_DIG_1, INPUT);    //Фототранзистор
  pinMode (PIN_MOSFET, OUTPUT);  //Цифровой пин управления MOSFET (электромагнитом)
  pinMode (PIN_EXT_DIG_2, OUTPUT);   //Инфракрасный светодиод
  attachInterrupt(PIN_BUTTON-2,ButtonPressed,RISING); //прерывание по кнопке
 
  TFT.Init(); //инициализация дисплея
  TFT.Print ("Demo Start", 20, 45, 2,  Display_Color_White); //пример вывода на дисплей
  digitalWrite(PIN_EXT_DIG_2, HIGH);
  TFT.fillScreen(Display_Color_Black);
} // ********************** END SETUP ***************************************
 
// ************************** LOOP ******************************************
void loop() {
    TFT.fillScreen(Display_Color_Black);
    uint32_t t1 = 0;   //время начала падения цепочки
    uint32_t t2 = 0;   //время срабатывания датчика от конца цепочки
    TFT.Print ("Настройка", 20, 25, 2,  Display_Color_Red);
    TFT.Print ("Состояние", 20, 45, 2,  Display_Color_White);
    TFT.Print ("датчика:", 20, 65, 2,  Display_Color_White);
    TFT.Printb (digitalRead(PIN_EXT_DIG_1), 120, 65, 2,  Display_Color_White,Display_Color_Black ); 
    TFT.Print ("Нажмите кнопку", 20, 100, 1,  Display_Color_White);
    TFT.Print ("для измерения", 20, 110, 1,  Display_Color_White);

    butPrs = true;
    while (butPrs) {}     //ожидание нажатия кнопки
    butPrs = true;

    TFT.fillScreen(Display_Color_Black);
    TFT.Print ("Измерение", 20, 45, 2,  Display_Color_White);

    digitalWrite(PIN_MOSFET,HIGH); // Поднимаем магнит, цепочка падает
    uint32_t t1 = millis(); // фиксируем время начала падения цепочки

    while (digitalRead (PIN_EXT_DIG_1) == 0){} // Ожидаем попадания цепочки на датчик

    uint32_t t2 = millis(); // Фиксируем время попадания цепочки на датчик
    digitalWrite(PIN_MOSFET, LOW); // Опускаем магнит
    uint32 t_T = t2-t1; // Расчитываем время падения
    TFT.Print("Время падения:", 20, 65, 1,  Display_Color_White);
    TFT.Print(T, 20, 85, 2,  Display_Color_White);
  
    while (butPrs){ } // Ожидаем следующего нажатия кнопки
    butPrs = true;  ______________
} // ************************** END LOOP ***********************
 
void ButtonPressed() { //функция обработки прерывания по кнопке
    static uint64_t millis_prev = 0;
    if (millis() > (millis_prev+400)) { //убираем дребезг кнопки
       butPrs =!butPrs; //глобальная переменная butPrs сигнализирует о прерывании
       millis_prev = millis();
    }
}  
