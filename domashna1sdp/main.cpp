#include "lib.h"
//#include "lib.cpp"
// /home/ismet/Downloads/fldr/mqu.csv

int main(int argc, char *argv[])
{
    char *path = argv[1];
    if(argc != 2) {std::cout << "Issue with finding the Path to the csv table!\n"; return -1;}
    
    std::cout << "Please, enter a query, or “exit” to leave.\n";

    std::string buffer;
    std::getline(std::cin, buffer);
    std::transform(buffer.begin(), buffer.end(), buffer.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });

    while (buffer.compare("EXIT") != 0)
    {
        std::stringstream ss(buffer);
        std::string dateOrExit, currencies;
        std::getline(ss, dateOrExit, ' ');
        std::getline(ss, currencies, '\n');

        try
        {
            if(currencies.substr(0,3).length() == 3 && currencies.substr(5,7).length())
            {

            
            exchange_rates er(path, currencies.c_str());
            time_t date = er.parseDateTime(dateOrExit.c_str());

            float course = er.for_date(date, currencies.c_str());

            std::cout << course << '\n';
            }
            else
            {
                std::cout << "WRONG FORMAT!!  Input should be: YYYY-mm-dd XXX YYY\n";
            }
        }
        catch (...)
        {
            std::cout << "BAAAAAAD!\n";
            break;
        }

        std::getline(std::cin, buffer);
        std::transform(buffer.begin(), buffer.end(), buffer.begin(),
                       [](unsigned char c)
                       { return std::toupper(c); });
    }

} // 1.17659998