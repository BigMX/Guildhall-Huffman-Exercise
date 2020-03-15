#include <string>


class fileManager
{
public:
    fileManager();
    ~fileManager();
    static void readFile(const char*, std::string&);    
    static void writeFile(const char*, std::string&);
private:
};