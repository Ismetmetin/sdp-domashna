#include "lib.h"

/*
    char *baseCurrency;
    std::vector<std::string> currencyNames;
    std::vector<std::vector<float>> currencyData;
    std::vector<time_t> dates;
*/
time_t exchange_rates::parseDateTime(const char *datetimeString)
{
    struct std::tm tm = {};
    strptime(datetimeString, "%Y-%m-%d", &tm);
    time_t timeStamp = mktime(&tm);
    return timeStamp;
}

exchange_rates::exchange_rates()
{
    baseCurrency = nullptr;
}

exchange_rates::exchange_rates(const char *path, const char *base_currency)
{
    from_csv(path, base_currency);
}

exchange_rates::~exchange_rates()
{
    delete[] baseCurrency;
}

exchange_rates::exchange_rates(const exchange_rates &copyFrom)
{
    baseCurrency = new char[4];
    std::strncpy(baseCurrency, copyFrom.baseCurrency, 4);

    this->currencyData = copyFrom.currencyData;
    this->currencyNames = copyFrom.currencyNames;
    this->dates = copyFrom.dates;
}

exchange_rates &exchange_rates::operator=(const exchange_rates &rhs)
{
    if (this != &rhs)
    {
        delete[] baseCurrency;
        baseCurrency = new char[4];
        std::strncpy(baseCurrency, rhs.baseCurrency, 4);

        this->currencyData = rhs.currencyData;
        this->currencyNames = rhs.currencyNames;
        this->dates = rhs.dates;
    }
    return *this;
}
void merge(std::vector<std::string> &currencyNames, std::vector<std::vector<float>> &currencyData, int left, int mid, int right);
void mergeSort(std::vector<std::string> &currencyNames, std::vector<std::vector<float>> &currencyData, int start, int end);


void exchange_rates::from_csv(const char *path, const char *base_currency)
{
    std::vector<std::vector<float>> newCurrencyData;
    std::vector<std::string> newCurrencyNames;
    std::vector<time_t> newDates;

    std::ifstream fs(path);
    if (!fs.is_open())
    {
        throw std::runtime_error("File did not open: " + std::string(path));
    }

    std::string header;
    if (!std::getline(fs, header))
    {
        throw std::runtime_error("Empty file or cannot read header");
    }

    std::stringstream ss(header);
    std::string cell;

    if (!std::getline(ss, cell, ','))
    {
        throw std::runtime_error("Bad file");
    }

    size_t newNameCount = 0;
    while (std::getline(ss, cell, ','))
    {
        cell.erase(std::remove(cell.begin(), cell.end(), '\"'), cell.end());
        cell.erase(0, cell.find_first_not_of(" \t"));
        cell.erase(cell.find_last_not_of(" \t") + 1);

        std::transform(cell.begin(), cell.end(), cell.begin(),
                       [](unsigned char c)
                       { return std::toupper(c); });
        newCurrencyNames.push_back(cell);
        ++newNameCount;
    }
    newCurrencyData.resize(newNameCount);

    std::string line;

    while (std::getline(fs, line))
    {

        std::stringstream line_ss(line);
        std::string date_str;

        std::getline(line_ss, date_str, ',');
        date_str.erase(std::remove(date_str.begin(), date_str.end(), '\"'), date_str.end());
        date_str.erase(0, date_str.find_first_not_of(" \t"));
        date_str.erase(date_str.find_last_not_of(" \t") + 1);

        time_t date = parseDateTime(date_str.c_str());
        newDates.push_back(date); // do tuk pushvam data vuv vectora

        for (size_t i = 0; i < newNameCount; ++i)
        {
            // trimvam kletkata
            std::getline(line_ss, cell, ',');
            cell.erase(0, cell.find_first_not_of(" \t\n\r"));
            cell.erase(cell.find_last_not_of(" \t\n\r") + 1);

            if (cell.compare("N/A") == 0)
            {
                newCurrencyData[i].push_back(std::nanf(""));
            }
            else if (cell.empty())
            {
                continue;
            }
            else
            {
                newCurrencyData[i].push_back(std::stof(cell));
            }
        }
    }

    fs.close();

    mergeSort(newCurrencyNames, newCurrencyData, 0, newCurrencyNames.size() - 1);

    // for (size_t i = 0; i < newNameCount - 1; ++i)
    // {
    //     for (size_t j = 0; j < newNameCount - 1 - i; ++j)
    //     {
    //         if (newCurrencyNames[j].compare(newCurrencyNames[j + 1]) > 0)
    //         {
    //             std::string tempName = newCurrencyNames[j];
    //             newCurrencyNames[j] = newCurrencyNames[j + 1];
    //             newCurrencyNames[j + 1] = tempName;

    //             std::vector<float> tempVector = newCurrencyData[j];
    //             newCurrencyData[j] = newCurrencyData[j + 1];
    //             newCurrencyData[j + 1] = tempVector;
    //         }
    //     }
    // } 

    baseCurrency = new char[4];
    for (size_t i = 0; i < 3; ++i)
    {
        baseCurrency[i] = std::toupper(base_currency[i]);
    }
    baseCurrency[3] = '\0';

    this->currencyData = newCurrencyData;
    this->currencyNames = newCurrencyNames;
    this->dates = newDates;
}

void merge(std::vector<std::string> &currencyNames, std::vector<std::vector<float>> &currencyData, int left, int mid, int right)
{

    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<std::string> subArrNames1(n1);
    std::vector<std::vector<float>> subArrCurr1(n1);
    std::vector<std::string> subArrNames2(n2);
    std::vector<std::vector<float>> subArrCurr2(n2);
    for (int i = 0; i < n1; i++)
    {
        subArrNames1[i] = currencyNames[left + i];
        subArrCurr1[i] = currencyData[left + i];
    }

    for (int j = 0; j < n2; j++)
    {
        subArrNames2[j] = currencyNames[left + j];
        subArrCurr2[j] = currencyData[left + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    while (i < n1 && j < n2)
    {
        if (currencyNames[j].compare(currencyNames[j + 1]) > 0)
        {
            currencyNames[k] = subArrNames1[i];
            currencyData[k] = subArrCurr1[i];
            i++;
            k++;
        }
        else
        {
            currencyNames[k] = subArrNames2[j];
            currencyData[k] = subArrCurr1[j];
            j++;
            k++;
        }
    }

    while (i < n1)
    {
        currencyNames[k] = subArrNames1[i];
        currencyData[k] = subArrCurr1[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        currencyNames[k] = subArrNames2[j];
        currencyData[k] = subArrCurr1[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<std::string> &currencyNames, std::vector<std::vector<float>> &currencyData, int start, int end)
{
    if (start >= end)
        return;

    int mid = start + (end - start) / 2;

    mergeSort(currencyNames, currencyData, start, mid);
    mergeSort(currencyNames, currencyData, mid + 1, end);

    merge(currencyNames, currencyData, start, mid, end);
}
float exchange_rates::for_date(time_t ymd, const char *currency) const
{
    std::string currenciesString(currency);
    std::stringstream ss(currenciesString);
    std::string currency1, currency2;
    std::getline(ss, currency1, ' ');
    std::getline(ss, currency2);

    currency1.erase(0, currency1.find_first_not_of(" \t\n\r"));
    currency1.erase(currency1.find_last_not_of(" \t\n\r") + 1);
    currency2.erase(0, currency2.find_first_not_of(" \t\n\r"));
    currency2.erase(currency2.find_last_not_of(" \t\n\r") + 1);

    // namiram index na data, namiram index na currencyName

    int currency1Index = binarySearchCurrencies(currency1);
    int currency2Index = binarySearchCurrencies(currency2);
    int dateIndex = dateBinarySearchIndex(ymd);
    if (dateIndex == -1)
    {
        throw std::runtime_error("No such date in database! ");
    }
    if (currency1Index == -1 || currency2Index == -1)
    {
        throw std::runtime_error("Unsupported currency!");
    }

    float value1 = (currency1Index == -2) ? 1 : this->currencyData[currency1Index][dateIndex];
    float value2 = (currency2Index == -2) ? 1 : this->currencyData[currency2Index][dateIndex];

    return value2 / value1;
}

const char *exchange_rates::base_currency() const
{
    return baseCurrency;
}

bool exchange_rates::is_supported(const char *currency) const
{
    std::string searchFor(currency);
    std::transform(searchFor.begin(), searchFor.end(), searchFor.begin(),
                   [](unsigned char c)
                   { return std::toupper(c); });

    int ans = binarySearchCurrencies(searchFor);
    return ans > 0 || ans == -2;
}

int exchange_rates::binarySearchCurrencies(const std::string currency) const
{
    if (currency.compare(this->baseCurrency) == 0)
        return -2;

    int left = 0;
    int right = currencyNames.size() - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        // std::cout<< left << ' ' << mid << ' ' << right << '\n';

        if (currencyNames[mid].compare(currency) == 0)
        {
            return mid;
        }
        else if (currencyNames[mid].compare(currency) < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return -1;
}

int exchange_rates::dateBinarySearchIndex(time_t ymd) const
{
    int left = 0;
    int right = dates.size() - 1;
    int rightBound = -1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        // std::cout<< left << ' ' << mid << ' ' << right << '\n';

        if (dates[mid] == ymd)
        {
            // std::cout << mid;
            return mid;
        }
        else if (dates[mid] > ymd)
        {
            left = mid + 1;
        }
        else
        {
            rightBound = mid;
            right = mid - 1;
        }
    }

    return rightBound;
}

void exchange_rates::printDates()
{
    for (int i = 0; i < dates.size(); ++i)
    {
        std::cout << i << ". " << dates[i] << '\n';
    }
}

void exchange_rates::printNames()
{
    for (int i = 0; i < currencyNames.size(); i++)
    {
        std::cout << i << ". " << currencyNames[i] << '\n';
    }
}

void exchange_rates::printTables()
{
    for (size_t i = 0; i < currencyData.size(); i++)
    {
        for (size_t j = 0; j < currencyData[i].size(); j++)
        {
            std::cout << currencyData[i][j] << ' ';
        }

        std::cout << '\n';
    }
}
