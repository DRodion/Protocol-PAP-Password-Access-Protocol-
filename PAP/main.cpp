#include <iostream>
#include <map>
#include "../cryptopp860/cryptlib.h"
#include "../cryptopp860/modes.h"
#include "../cryptopp860/filters.h"
#include "../cryptopp860/sha.h"
#include "../cryptopp860/base64.h"

using namespace CryptoPP;
using namespace std;

//функция хеширования sha256 с выводом base64
string SHA256HashString(string aString) {
    string digest;
    SHA256 hash;

    StringSource foo(aString, true, new HashFilter(hash, new Base64Encoder(new StringSink(digest))));

    return digest;
}

// класс сервер
class Server
{
private:
    // контейнер для хранения данных вида [ключ — значение], [логин - пароль]
    map <string, string> db;
public:
    // функция регистрации нового пользователя
    int registration(string login, string password) 
    {
        const auto found = db.find(login);
        if (found == db.cend()) {
            db[login] = SHA256HashString(password); // шифрование пароля с помощью sha256
            for (auto it = db.begin(); it != db.end(); ++it){
                cout << "Server: Регистрация прошла. Данные в базе db: '" << (*it).first << "' : " << (*it).second << endl;
                return true;
            }
        }
        else {
            cout << "Server: Error. Пользователь уже зарегистрирован с таким логином." << endl;
            return false;
        }
           
    }
    // функция проверки аутентификации
    int auth(string login, string password){
        const auto found = db.find(login);

        if (found != db.cend()) {
            if (db[login] != SHA256HashString(password)) {
                cout << "Server: Неверный пароль = " << SHA256HashString(password) << endl;
                return false;
            }
            else {
                cout << "Server: Успешная аутентификация." << endl;
                return true;
            } 
        }
        else {
            cout << "Server: Неверный логин = '" << login << "' " << endl;
            return false;
        }
    }
};

//Класс Пользователь
class User
{
public:
    // функция регистрации пользователя
    void regisration_user(Server& server, string login, string password) {
        
        string pass = SHA256HashString(password);
        cout << "User: Исходные данные. Логин = '" << login << "', пароль = " << pass << endl;

        cout << "User: Регистрация..." << endl;
        bool status_registration = server.registration(login, password); //вызов функци регистрации с Server
        if (status_registration == true) {
            cout << "User: Успешная регистрация." << endl;
        }
        else {
            cout << "User: Ошибка при регистрации." << endl;
        }
    }

    // функция аутентификации пользователя
    void auth_user(Server& server, string login, string password) {
        cout << "User: Аутентификация... Пароль: " << SHA256HashString(password) << endl;
        bool status_auth = server.auth(login, password); //вызов функци аутентификации с Server
        if (status_auth == true) {
            cout << "User: Успешная аутентификация" << endl;
        }
        else {
            cout << "User: Ошибка при аутентификации" << endl;
        }
    }
};


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    Server objPAPServer; // объявление класса Server
    User objPAPUser; // объявление класса User

    // Корректные данные
    //objPAPUser.regisration_user(objPAPServer, "Alice", "qr1233");
    //objPAPUser.auth_user(objPAPServer, "Alice", "qr1233");
      
 
    //Некорректный пароль
    objPAPUser.regisration_user(objPAPServer, "Alice", "1233");
    objPAPUser.auth_user(objPAPServer, "Alice", "1258963");
    

    system("pause");
    return 0;
}
