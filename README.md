# Myproject
Программное обеспечение мехатронных и робототехнических систем Курушин Д.С. ПРТ-21-1б Пикулев Максим Выймов Станислав
Итоговый проект по робототехнике
Суть проекта: 
Вывод на отдельный экран данных о загрузке центрального процессора и памяти компьютера, к которому подключена данная схема. 

Требуемые компоненты:
- Arduino Uno
- Экран с i2c
- 4 провода «папа мама»
- соединительный кабель

Как запустить?
- загрузить на плату скетч, при помощи программы Arduino (должна быть установлена библиотека LiquidCrystal_I2C), из папки robotics_program_for_arduino. Данный скетч просто выводит на экран информацию, полученную от компьютера. 
- после загрузки скетча на плату НЕ отсоединяем соединительный провод, так как по нему будут передаваться данные 
- файл proekt с кодом на С++. Данная программа отправляет на плату информацию, которую нужно вывести на экран, подсоединённый к плате.  Запускаем proekt.cpp.
- В открывшемся коде нужно в 7 строке поменять порт, к которому подлечена arduino, на актуальный

- Запускаем проект, пока программа запущена данные на экране будут обновляться раз в 2 секунды (откроется консоль с сообщением «нажмите любую клавишу для продолжения . . .», но с ней ничего не надо делать)

Работу выполняли: 
- Выймов Станислав ПРТ-21-1б
- Пикулев Максим Прт-21-1б
