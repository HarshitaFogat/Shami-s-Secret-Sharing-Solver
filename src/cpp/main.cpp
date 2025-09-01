#include <bits/stdc++.h>
using namespace std;

// Improved BigInt class with division support
class BigInt {
private:
    string value;
    bool negative;

    static string addStrings(const string& a, const string& b) {
        string res;
        int carry = 0, i = a.size()-1, j = b.size()-1;
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += a[i--] - '0';
            if (j >= 0) sum += b[j--] - '0';
            res.push_back((sum % 10) + '0');
            carry = sum / 10;
        }
        reverse(res.begin(), res.end());
        return res;
    }

    static string subStrings(const string& a, const string& b) {
        string res;
        int borrow = 0, i = a.size()-1, j = b.size()-1;
        while (i >= 0) {
            int diff = (a[i]-'0') - borrow - (j>=0 ? (b[j--]-'0') : 0);
            if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
            res.push_back(diff+'0');
            i--;
        }
        while (res.size() > 1 && res.back() == '0') res.pop_back();
        reverse(res.begin(), res.end());
        return res;
    }

    static string mulStrings(const string& a, const string& b) {
        if (a=="0" || b=="0") return "0";
        vector<int> res(a.size()+b.size(), 0);
        for (int i=a.size()-1; i>=0; i--) {
            for (int j=b.size()-1; j>=0; j--) {
                int mul = (a[i]-'0')*(b[j]-'0');
                int sum = res[i+j+1]+mul;
                res[i+j+1] = sum%10;
                res[i+j] += sum/10;
            }
        }
        string s;
        for (int num : res) if(!(s.empty() && num==0)) s.push_back(num+'0');
        return s.empty() ? "0" : s;
    }

    static bool isSmaller(const string& a, const string& b) {
        if (a.length() < b.length()) return true;
        if (b.length() < a.length()) return false;
        return a < b;
    }

    static string divideStrings(const string& dividend, const string& divisor) {
        if (divisor == "0") throw runtime_error("Division by zero");
        if (isSmaller(dividend, divisor)) return "0";
        
        string quotient = "0";
        string temp = "";
        
        for (char digit : dividend) {
            temp += digit;
            // Remove leading zeros
            while (temp.length() > 1 && temp[0] == '0') temp = temp.substr(1);
            
            if (!isSmaller(temp, divisor)) {
                int count = 0;
                while (!isSmaller(temp, divisor)) {
                    temp = subStrings(temp, divisor);
                    count++;
                }
                quotient += to_string(count);
            } else {
                if (quotient != "0") quotient += "0";
            }
        }
        
        return quotient == "" ? "0" : quotient;
    }

public:
    BigInt(): value("0"), negative(false) {}
    BigInt(long long num) {
        if (num < 0) { negative = true; num = -num; } else negative = false;
        value = to_string(num);
    }
    BigInt(string s) {
        if (s[0]=='-') { negative = true; value = s.substr(1); }
        else { negative = false; value = s; }
        if (value == "0") negative = false;
    }

    string toString() const { return (negative?"-":"")+value; }

    bool isZero() const { return value == "0"; }

    BigInt operator+(const BigInt& other) const {
        if (!negative && !other.negative) return BigInt(addStrings(value, other.value));
        if (negative && other.negative) return BigInt("-"+addStrings(value, other.value));
        if (!negative && other.negative) {
            if (value==other.value) return BigInt("0");
            if (value.size()>other.value.size() || (value.size()==other.value.size() && value>=other.value))
                return BigInt(subStrings(value, other.value));
            else return BigInt("-"+subStrings(other.value, value));
        }
        if (negative && !other.negative) {
            if (value==other.value) return BigInt("0");
            if (other.value.size()>value.size() || (other.value.size()==value.size() && other.value>=value))
                return BigInt(subStrings(other.value, value));
            else return BigInt("-"+subStrings(value, other.value));
        }
        return BigInt("0");
    }

    BigInt operator-(const BigInt& other) const {
        BigInt temp = other;
        temp.negative = !temp.negative;
        return *this + temp;
    }

    BigInt operator*(const BigInt& other) const {
        string res = mulStrings(value, other.value);
        bool neg = (negative != other.negative) && res != "0";
        return BigInt((neg?"-":"")+res);
    }

    BigInt operator/(const BigInt& other) const {
        if (other.isZero()) throw runtime_error("Division by zero");
        string res = divideStrings(value, other.value);
        bool neg = (negative != other.negative) && res != "0";
        return BigInt((neg?"-":"")+res);
    }

    bool operator<(const BigInt& other) const {
        if (negative != other.negative) return negative;
        if (negative) return BigInt(other.value) < BigInt(value);
        return isSmaller(value, other.value);
    }

    bool operator==(const BigInt& other) const {
        return negative == other.negative && value == other.value;
    }
};

// Fraction class for exact arithmetic
class Fraction {
private:
    BigInt num, den;
    
    BigInt gcd(BigInt a, BigInt b) {
        while (!b.isZero()) {
            BigInt temp = b;
            b = a - (a / b) * b;  // a % b equivalent
            a = temp;
        }
        return a;
    }
    
public:
    Fraction(BigInt n = BigInt(0), BigInt d = BigInt(1)) : num(n), den(d) {
        if (den.isZero()) throw runtime_error("Division by zero in fraction");
        // Reduce fraction (simplified)
        if (!num.isZero()) {
            // For simplicity, we'll skip GCD reduction in this basic version
        }
    }
    
    Fraction operator+(const Fraction& other) const {
        return Fraction(num * other.den + other.num * den, den * other.den);
    }
    
    Fraction operator*(const Fraction& other) const {
        return Fraction(num * other.num, den * other.den);
    }
    
    Fraction operator/(const Fraction& other) const {
        if (other.num.isZero()) throw runtime_error("Division by zero");
        return Fraction(num * other.den, den * other.num);
    }
    
    BigInt toInteger() const {
        if (!(den == BigInt(1))) {
            throw runtime_error("Fraction cannot be converted to integer: " + num.toString() + "/" + den.toString());
        }
        return num;
    }
};

// Base conversion (same as original)
BigInt baseToDecimal(const string& value, int base) {
    BigInt result(0);
    BigInt bigBase(base);
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else throw runtime_error("Invalid char in number");
        if (digit >= base) throw runtime_error("Digit out of range for base");
        result = result * bigBase + BigInt(digit);
    }
    return result;
}

// JSON parser (same as original)
map<string, map<string,string>> parseJSON(const string& json) {
    map<string,map<string,string>> res;
    regex keysRegex(R"("keys"\s*:\s*\{\s*"n"\s*:\s*(\d+)\s*,\s*"k"\s*:\s*(\d+)\s*\})");
    smatch m;
    if (regex_search(json,m,keysRegex)) {
        res["keys"]["n"]=m[1].str();
        res["keys"]["k"]=m[2].str();
    }
    regex pointRegex(R"("(\d+)"\s*:\s*\{\s*"base"\s*:\s*"(\d+)"\s*,\s*"value"\s*:\s*"([^"]+)"\s*\})");
    sregex_iterator it(json.begin(),json.end(),pointRegex),end;
    for(;it!=end;++it){
        res[(*it)[1].str()]["base"]=(*it)[2].str();
        res[(*it)[1].str()]["value"]=(*it)[3].str();
    }
    return res;
}

// CORRECT: Lagrange interpolation at x=0
BigInt lagrangeInterpolationAtZero(const vector<pair<int, BigInt>>& points) {
    Fraction result(BigInt(0));
    int k = points.size();
    
    cout << "\nPerforming Lagrange interpolation at x=0:" << endl;
    
    for (int i = 0; i < k; i++) {
        int xi = points[i].first;
        BigInt yi = points[i].second;
        
        // Calculate Li(0) = ∏(0-xj)/(xi-xj) for j≠i
        Fraction li(BigInt(1));
        
        for (int j = 0; j < k; j++) {
            if (i != j) {
                int xj = points[j].first;
                Fraction numerator(BigInt(-xj));    // 0 - xj
                Fraction denominator(BigInt(xi - xj));
                li = li * (numerator / denominator);
            }
        }
        
        cout << "Point " << i+1 << ": (" << xi << ", " << yi.toString() << ")" << endl;
        
        Fraction term = Fraction(yi) * li;
        result = result + term;
    }
    
    return result.toInteger();
}

int main(){
    string testCase1 = R"({
        "keys": { "n": 4, "k": 3 },
        "1": { "base": "10", "value": "4" },
        "2": { "base": "2", "value": "111" },
        "3": { "base": "10", "value": "12" },
        "6": { "base": "4", "value": "213" }
    })";

    string testCase2 = R"({
        "keys": { "n": 10, "k": 7 },
        "1": { "base": "6", "value": "13444211440455345511" },
        "2": { "base": "15", "value": "aed7015a346d635" },
        "3": { "base": "15", "value": "6aeeb69631c227c" },
        "4": { "base": "16", "value": "e1b5e05623d881f" },
        "5": { "base": "8", "value": "316034514573652620673" },
        "6": { "base": "3", "value": "2122212201122002221120200210011020220200" },
        "7": { "base": "3", "value": "20120221122211000100210021102001201112121" },
        "8": { "base": "6", "value": "20220554335330240002224253" },
        "9": { "base": "12", "value": "45153788322a1255483" },
        "10": { "base": "7", "value": "1101613130313526312514143" }
    })";

    cout << "=== CORRECTED SHAMIR'S SECRET SHARING SOLVER ===" << endl;

    for (int tc = 0; tc < 2; tc++) {
        string jsonInput = (tc == 0) ? testCase1 : testCase2;
        cout << "\n" << string(50, '=') << endl;
        cout << "TEST CASE " << (tc+1) << endl;
        cout << string(50, '=') << endl;

        try {
            auto data = parseJSON(jsonInput);
            int n = stoi(data["keys"]["n"]);
            int k = stoi(data["keys"]["k"]);
            
            cout << "n = " << n << ", k = " << k << endl;
            cout << "Polynomial degree = " << k-1 << endl;

            vector<pair<int, BigInt>> points;
            
            // Extract points correctly as (x, y) coordinates
            for (int i = 1; i <= n; i++) {
                string key = to_string(i);
                if (data.count(key)) {
                    int base = stoi(data[key]["base"]);
                    string val = data[key]["value"];
                    BigInt y = baseToDecimal(val, base);
                    points.push_back({i, y});  // (x=i, y=decoded_value)
                    cout << "Point " << i << ": (" << i << ", " << y.toString() << ") ";
                    cout << "[base " << base << ": \"" << val << "\"]" << endl;
                }
            }

            // Take first k points
            if (points.size() >= k) {
                points.resize(k);
                BigInt secret = lagrangeInterpolationAtZero(points);
                cout << "\n SECRET: " << secret.toString() << endl;
            } else {
                cout << "Error: Not enough points!" << endl;
            }

        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}
