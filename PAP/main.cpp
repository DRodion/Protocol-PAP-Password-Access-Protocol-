#include <iostream>
#include <map>
#include "../cryptopp860/cryptlib.h"
#include "../cryptopp860/modes.h"
#include "../cryptopp860/filters.h"
#include "../cryptopp860/sha.h"
#include "../cryptopp860/base64.h"

using namespace CryptoPP;
using namespace std;

//������� ����������� sha256 � ������� base64
string SHA256HashString(string aString) {
    string digest;
    SHA256 hash;

    StringSource foo(aString, true, new HashFilter(hash, new Base64Encoder(new StringSink(digest))));

    return digest;
}

// ����� ������
class Server
{
private:
    // ��������� ��� �������� ������ ���� [���� � ��������], [����� - ������]
    map <string, string> db;
public:
    // ������� ����������� ������ ������������
    int registration(string login, string password) 
    {
        const auto found = db.find(login);
        if (found == db.cend()) {
            db[login] = SHA256HashString(password); // ���������� ������ � ������� sha256
            for (auto it = db.begin(); it != db.end(); ++it){
                cout << "Server: ����������� ������. ������ � ���� db: '" << (*it).first << "' : " << (*it).second << endl;
                return true;
            }
        }
        else {
            cout << "Server: Error. ������������ ��� ��������������� � ����� �������." << endl;
            return false;
        }
           
    }
    // ������� �������� ��������������
    int auth(string login, string password){
        const auto found = db.find(login);

        if (found != db.cend()) {
            if (db[login] != SHA256HashString(password)) {
                cout << "Server: �������� ������ = " << SHA256HashString(password) << endl;
                return false;
            }
            else {
                cout << "Server: �������� ��������������." << endl;
                return true;
            } 
        }
        else {
            cout << "Server: �������� ����� = '" << login << "' " << endl;
            return false;
        }
    }
};

//����� ������������
class User
{
public:
    // ������� ����������� ������������
    void regisration_user(Server& server, string login, string password) {
        
        string pass = SHA256HashString(password);
        cout << "User: �������� ������. ����� = '" << login << "', ������ = " << pass << endl;

        cout << "User: �����������..." << endl;
        bool status_registration = server.registration(login, password); //����� ������ ����������� � Server
        if (status_registration == true) {
            cout << "User: �������� �����������." << endl;
        }
        else {
            cout << "User: ������ ��� �����������." << endl;
        }
    }

    // ������� �������������� ������������
    void auth_user(Server& server, string login, string password) {
        cout << "User: ��������������... ������: " << SHA256HashString(password) << endl;
        bool status_auth = server.auth(login, password); //����� ������ �������������� � Server
        if (status_auth == true) {
            cout << "User: �������� ��������������" << endl;
        }
        else {
            cout << "User: ������ ��� ��������������" << endl;
        }
    }
};


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");
    Server objPAPServer; // ���������� ������ Server
    User objPAPUser; // ���������� ������ User

    // ���������� ������
    //objPAPUser.regisration_user(objPAPServer, "Alice", "qr1233");
    //objPAPUser.auth_user(objPAPServer, "Alice", "qr1233");
      
 
    //������������ ������
    objPAPUser.regisration_user(objPAPServer, "Alice", "1233");
    objPAPUser.auth_user(objPAPServer, "Alice", "1258963");
    

    system("pause");
    return 0;
}
