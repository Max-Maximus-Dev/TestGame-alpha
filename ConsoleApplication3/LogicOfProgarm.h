#pragma once
#include <iostream>
// Classes
class User {};
class Administrator : public User {};
class Guest : public User {};
class Test {};
class Question {};
class Answer {};
class categoryOfTests {};
// Classes

// Functions
void registerUser() {}
void loginUser() {}
void passTest() {}
// Functions

// Administrator functions
void createGuestAccount() {}
void deleteGuestAccount() {}
void manageLoginPasswordGuest() {}
void showStatistics() {} 
void addCategoryOfTests() {}
void deleteCategoryOfTests() {}
void addTest() {}
void deleteTest() {}
void addQuestionToTest() {}
void deleteQuestionFromTest() {}
// Administrator functions

void SerializeData() {}
void DeserializeData() {}