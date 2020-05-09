#include <unordered_map>
#include <string>
#include "http_protocol.h"
#include "common.h"
using namespace std;

void say_hello(request_rec *r) {
    ap_rputs("Thanks for your come!Nice to meet you.", r);
}

void pay_money(request_rec *r) {
    ap_rputs("You Need pay money for your home.", r);
}

unordered_map<string, CSP_HANDLER> service = {
    {"sayhello.csp", say_hello},
    {"paymoney.csp", pay_money}
};

