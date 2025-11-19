#ifndef THRONG_H
#define THRONG_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

namespace mystr {

typedef struct mystr_t {
    char            *str;
    u_int64_t       len;
    unsigned long   hash;

    /**
     * @brief Сравнивает текущий объект с другим экземпляром mystr_t
     * @param second Указатель на второй объект mystr_t
     * @return true если строки идентичны, false иначе
     */
    bool is_same(const mystr_t *second) {
        return second && str && second->str && hash == second->hash && strcmp(str, second->str) == 0;
    }
} mystr_t;

/**
 * @brief Создаёт структуру mystr_t на основе переданной C-строки
 * @param str Указатель на строку, чья память управляется вызывающей стороной
 * @return Инициализированная структура mystr_t или пустая, если str == NULL
 */
mystr_t construct(char *str);

/**
 * @brief Выводит строку посимвольно в stdout
 * @param str Указатель на строку для вывода
 * @return 0 при успешном выполнении
 */
int put(const mystr_t *str);

/**
 * @brief Ищет первое вхождение символа в строке
 * @param str Указатель на строку для поиска
 * @param c Символ для поиска
 * @return Указатель на найденный символ или NULL если не найден
 * @note Аналог функции strchr() из <string.h>.
 */
const char * find_char(const mystr_t *str, char c);

/**
 * @brief Ищет последнее вхождение символа в строке
 * @param str Указатель на строку
 * @param c Символ для поиска
 * @return Указатель на найденный символ или NULL, если символ отсутствует
 */
const char * find_last_char(const mystr_t *str, char c);

/**
 * @brief Ищет первое вхождение любого символа из accept
 * @param str Указатель на строку для поиска
 * @param accept Набор символов, которые требуется найти
 * @return Указатель на найденный символ или NULL, если совпадений нет
 */
const char * find_any(const mystr_t *str, const mystr_t *accept);

/**
 * @brief Подсчитывает длину начального сегмента str, содержащего только символы из accept
 * @param str Указатель на строку
 * @param accept Набор допустимых символов
 * @return Длина сегмента
 */
size_t span(const mystr_t *str, const mystr_t *accept);

/**
 * @brief Подсчитывает длину начального сегмента str, НЕ содержащего символы из reject
 * @param str Указатель на строку
 * @param reject Набор запрещённых символов
 * @return Длина сегмента
 */
size_t cspan(const mystr_t *str, const mystr_t *reject);

/**
 * @brief Вычисляет длину строки
 * @param str Указатель на строку
 * @return Длина строки в символах (без учета '\0')
 * @note Аналог функции strlen() из <string.h>.
 */
size_t len(const mystr_t *str);

/**
 * @brief Копирует строку src в dst
 * @param dst Указатель на буфер назначения
 * @param src Указатель на исходную строку
 * @return Указатель на dst
 * @note Аналог функции strcpy() из <string.h>. 
 */
mystr_t * copy(mystr_t *dst, const mystr_t *src);

/**
 * @brief Копирует не более count символов из src в dst
 * @param dst Указатель на буфер назначения
 * @param src Указатель на исходную строку
 * @param count Максимальное количество символов для копирования
 * @return Указатель на dst или NULL если count = 0
 * @note Аналог функции strncpy() из <string.h>. 
 */
mystr_t * ncopy(mystr_t *dst, const mystr_t *src, size_t count);

/**
 * @brief Присоединяет строку src к концу строки dst
 * @param dst Указатель на строку назначения
 * @param src Указатель на присоединяемую строку
 * @return Указатель на dst
 * @note Аналог функции strcat() из <string.h>. 
 */
mystr_t * concat(mystr_t *dst, const mystr_t *src);

/**
 * @brief Присоединяет не более count символов из src к концу строки dst
 * @param dst Указатель на строку назначения
 * @param src Указатель на присоединяемую строку
 * @param count Максимальное количество символов для присоединения
 * @return Указатель на dst или NULL если count = 0
 * @note Аналог функции strncat() из <string.h>. 
 */
mystr_t * nconcat(mystr_t *dst, const mystr_t *src, size_t count);

/**
 * @brief Создает новую строку путем конкатенации двух строк
 * @param first Указатель на первую строку
 * @param second Указатель на вторую строку
 * @return Указатель на новую выделенную строку (требует освобождения памяти)
 */
mystr_t dupe_concat(const mystr_t *first, const mystr_t *second);

/**
 * @brief Читает строку из файлового потока до символа новой строки
 * @param str Указатель на буфер для чтения строки
 * @param stream Файловый поток для чтения
 * @return Указатель на str или NULL при ошибке
 * @note Похожа на функцию fgets() из <stdio.h>, но без ограничения размера буфера.
 *       Рекомендуется использовать стандартную fgets() с проверкой размера буфера.
 */
mystr_t * fget(mystr_t *str, FILE *stream);

/**
 * @brief Читает не более count символов из файлового потока до символа новой строки
 * @param str Указатель на буфер для чтения строки
 * @param count Максимальное количество символов для чтения
 * @param stream Файловый поток для чтения
 * @return Указатель на str или NULL при ошибке
 * @note Аналог функции fgets() из <stdio.h>. 
 */
mystr_t * fnget(mystr_t *str, size_t count, FILE *stream);

/**
 * @brief Создает дубликат строки в динамической памяти
 * @param str Указатель на исходную строку
 * @return Указатель на новую выделенную строку (требует освобождения памяти)
 * @note Аналог функции strdup() из <string.h>. 
 */
mystr_t dupe(const mystr_t *str);

/**
 * @brief Читает строку переменной длины из файлового потока с автоматическим выделением памяти
 * @param ptr Указатель на указатель буфера (может быть перевыделен)
 * @param len Указатель на размер буфера (может быть изменен)
 * @param stream Файловый поток для чтения
 * @return Количество прочитанных символов или -1 при ошибке/EOF
 * @note Аналог функции getline() из <stdio.h> (POSIX). 
 */
ssize_t getline(mystr_t *line, FILE *stream);

/**
 * @brief Поэлементно сравнивает две строки лексикографически
 * @param first Указатель на первую строку
 * @param second Указатель на вторую строку
 * @return положительное значение если first > second, отрицательное значение если first < second, 0 если равны
 * @note Аналог функции strcmp() из <string.h>. 
 */
int comp(const mystr_t *first, const mystr_t *second);

/**
 * @brief Сравнивает две строки лексикографически до указанного символа-границы (сравниваются подстроки до первого включения символа-границы)
 * @param first Указатель на первую строку
 * @param second Указатель на вторую строку
 * @param final Символ, до которого производится сравнение
 * @return положительное значение если first > second, отрицательное значение если first < second, 0 если равны
 */
int comp_to(const mystr_t *first, const mystr_t *second, char final);

/**
 * @brief Поэлементно сравнивает не более size символов двух строк
 * @param first Указатель на первую строку
 * @param second Указатель на вторую строку
 * @param size Максимальное количество символов для сравнения
 * @return 1 если first > second, -1 если first < second, 0 если равны
 * @note Аналог функции strncmp() из <string.h>. 
 */
int ncomp(const mystr_t *first, const mystr_t *second, size_t size);

/**
 * @brief Создает новую строку путем повторения исходной строки count раз
 * @param src Указатель на исходную строку
 * @param count Количество повторений
 * @return Указатель на новую выделенную строку (требует освобождения памяти)
 */
mystr_t mult(const mystr_t *src, size_t count);

/**
 * @brief Ищет подстроку в строке используя rolling hash алгоритм
 * @param haystack Указатель на строку для поиска
 * @param needle Указатель на искомую подстроку
 * @return Указатель на найденную подстроку или NULL если не найдена
 * @note Аналог функции strstr() из <string.h>. 
 */
const char * find_str(const mystr_t *haystack, const mystr_t *needle);

/**
 * @brief Подсчитывает количество вхождений символа в строке ограниченной длины
 * @param haystack Указатель на строку для поиска
 * @param haystack_len Длина строки haystack
 * @param needle Искомый символ
 * @return Количество найденных символов или -1 при ошибке
 */
ssize_t count_needle_in_haystack(const mystr_t *haystack, char needle);

/**
 * @brief Заменяет все вхождения символа src на символ dst в строке ограниченной длины
 * @param haystack Указатель на строку для замены
 * @param haystack_len Длина строки haystack
 * @param src Заменяемый символ
 * @param dst Символ для замены
 * @return Количество замененных символов или -1 при ошибке
 */
ssize_t replace_needle_in_haystack(mystr_t *haystack, char src, char dst);

/**
 * @brief Перемещает указатель к ближайшему алфавитному символу относительно текущей позиции
 *
 * Алфавитный символ определяется как символ, для которого isalpha() возвращает ненулевое
 * значение (в соответствии с текущей локалью). Функция изменяет переданный указатель
 * на месте.
 *
 * @param ptr Адрес указателя на текущую позицию в строке; по завершении будет указывать
 *            на найденный алфавитный символ
 * @param backword Направление поиска: 0 — вперёд (к концу строки), ненулевое — назад (к началу)
 * @note Ожидается, что в выбранном направлении располагается валидная область памяти
 *       внутри одной нуль-терминированной строки; выход за её границы приводит к
 *       неопределённому поведению.
 */
size_t move_ptr_to_first_alpha_symbol(const mystr_t *str, size_t start, int backword);

/**
 * @brief Перемещает указатель к ближайшему символу, отличному от пробельного, относительно текущей позиции
 *
 * Пробельные символы определяются как символы, для которых isspace() возвращает ненулевое
 * значение (в соответствии с текущей локалью). Функция изменяет переданный указатель
 * на месте.
 *
 * @param ptr Адрес указателя на текущую позицию в строке; по завершении будет указывать
 *            на первый непробельный символ
 * @param backword Направление поиска: 0 — вперёд (к концу строки), ненулевое — назад (к началу)
 * @note Ожидается, что в выбранном направлении располагается валидная область памяти
 *       внутри одной нуль-терминированной строки; выход за её границы приводит к
 *       неопределённому поведению.
 */
size_t move_ptr_to_first_not_space_symbol(const mystr_t *str, size_t start, int backword);

/**
 * @brief Проверяет, что строка не является NULL и не пустой
 * @param str Указатель на строку для проверки
 * @return Ненулевое значение если строка не пустая, 0 если пустая или NULL
 */
int is_not_empty(const mystr_t *str);

/**
 * @brief Вычисляет хэш строки по алгоритму djb2 (Dan Bernstein)
 *
 * Данный алгоритм (k=33) был впервые описан Дэном Бернстейном в comp.lang.c.
 * Другая версия этого алгоритма (предпочитаемая Бернстейном) использует XOR:
 * hash(i) = hash(i - 1) * 33 ^ str[i];
 * Магия числа 33 (почему оно работает лучше многих других констант)
 * так и не была должным образом объяснена.
 *
 * @param str Указатель на строку для хэширования
 * @return Хэш-значение строки
 */
unsigned long djb2(const mystr_t *str);

/**
 * @brief Вычисляет хэш строки по алгоритму sdbm
 *
 * Данный алгоритм был создан для библиотеки базы данных sdbm
 * (публично доступная реимплементация ndbm).
 * Было обнаружено, что он хорошо перемешивает биты, обеспечивая лучшее
 * распределение ключей и меньшее количество коллизий.
 * Это также хорошая универсальная хэш-функция с хорошим распределением.
 * Фактическая функция: hash(i) = hash(i - 1) * 65599 + str[i];
 * Ниже представлена более быстрая версия, используемая в gawk.
 * Магическая простая константа 65599 (2^6 + 2^16 - 1) была выбрана
 * экспериментально среди множества различных констант.
 * Это один из алгоритмов, используемых в Berkeley DB и других местах.
 *
 * @param str Указатель на строку для хэширования
 * @return Хэш-значение строки
 *
 * @details Значение 0 принято как poison и никакой хеш не может его иметь
 */
unsigned long sdbm(const mystr_t *str);

}

#endif // THRONG_H