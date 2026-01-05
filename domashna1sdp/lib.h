#ifndef CURR_LIBRARY_H
#define CURR_LIBRARY_H
#include <iostream>
#include <vector>
#include <fstream>
#include <exception>
#include <ctime>
#include <string>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <cstring>
#include <iomanip>

class exchange_rates
{
    char *baseCurrency;
    std::vector<std::string> currencyNames;
    std::vector<std::vector<float>> currencyData;
    std::vector<time_t> dates;

public:

    void printDates();

    void printNames();

    void printTables();

    //
    time_t parseDateTime(const char* datetimeString);

    exchange_rates();

    exchange_rates(const char *path, const char *base_currency);

    ~exchange_rates();
    

    exchange_rates(const exchange_rates &copyFrom);

    exchange_rates& operator=(const exchange_rates &rhs);

    void from_csv(const char *path, const char *base_currency);

    float for_date(time_t ymd, const char *currency) const;
    
    const char *base_currency() const;

    bool is_supported(const char *currency) const;

    int binarySearchCurrencies(const std::string currency) const;
    
    int dateBinarySearchIndex(time_t ymd) const;
    
};

#endif