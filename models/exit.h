// Copyright 2017 Sean Bollin

#ifndef MODELS_EXIT_H_
#define MODELS_EXIT_H_
#include <string>
#include <unordered_map>

class Exit {
 public:
    explicit Exit(const std::string &direction, unsigned int toId)
        : direction_(direction),
          toId_(toId) {}
    std::string getExitDisplay() const;
 private:
    static const std::unordered_map<std::string, std::string> FULL_CARDINALS;

    std::string direction_;
    unsigned int toId_;
};

#endif  // MODELS_EXIT_H_
