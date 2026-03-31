#ifndef GAME_H
#define GAME_H

#include <string>

struct Node;
struct GameState;
class Card;
class Pile;

/**
 * Skill card type
 * MULTIPLIER: Increases the score multiplier for the next score card
 * HEAD_BUTT: Moves the card at the bottom (tail) of the discard pile to the top (head) of the draw pile
 */
enum SkillCardType {
    MULTIPLIER,
    HEAD_BUTT,
};

/**
 * Card base class
 * TODO: Add member variables and functions if needed
 */
class Card {
public:
    explicit Card() = default;
    virtual ~Card() = default;
    // Execute card effect, implemented by derived classes. You need to implement the specific effect of the current card type in each derived class
    virtual void play(GameState& game_state) = 0;
};

/**
 * Score card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class ScoreCard: public Card {
private:
    int point_;
public:
    explicit ScoreCard(int point);
    void play(GameState &game_state) override;
    ~ScoreCard() override = default;
};

/**
 * Skill card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class SkillCard: public Card {
private:
    SkillCardType skill_card_type_;
public:
    explicit SkillCard(SkillCardType skill_card_type);
    void play(GameState &game_state) override;
    ~SkillCard() override = default;
};

/**
 * Power card class
 * TODO: Add member variables and functions as needed
 * You can modify the destructor if needed
 */
class PowerCard: public Card {
private:
    int plus_count_;
public:
    explicit PowerCard(int plus_count);
    void play(GameState &game_state) override;
    ~PowerCard() override = default;
};

/**
 * Linked list node structure
 * TODO: Add or modify member variables and functions as needed
 */
struct Node {
    Card* card;
    Node* next;

    Node(Card* c) : card(c), next(nullptr) {}
};

/**
 * Pile class
 * All three piles (draw pile, discard pile, hand pile) are represented using this class
 * TODO: Modify as needed, add member variables and functions
 */
class Pile {
private:
    int size_;
    Node* head_;
    Node* tail_;

public:
    friend void outShuffle(GameState&);
    friend void inShuffle(GameState&);
    friend void oddEvenShuffle(GameState&);
    friend class GameController;

    // Constructor and destructor declarations
    // Please implement them in the later part of the file
    Pile();
    ~Pile();

    // Return pile size
    int size() const {
        return size_;
    }

    // Check if pile is empty
    bool empty() const {
        return size_ == 0;
    }

    // Add a card to the end of the pile
    // This function is used to initialize the pile in main.cpp
    // Please implement this function in the later part of the file
    void appendCard(Card* card);

    // Helper functions
    Card* removeFromHead();
    void addToTail(Card* card);
    Card* removeFromTail();
    void addToHead(Card* card);
    Card* removeAt(int index); // index starts from 1
};

/**
 * Game state structure
 * Stores current game state information such as discard pile, draw pile, hand pile status, total score, current multiplier, etc.
 * TODO: Add member variables and functions as needed
 */
struct GameState {
    Pile hand_{};            // Hand pile
    Pile draw_pile_{};       // Draw pile
    Pile discard_pile_{};    // Discard pile

    int total_score_ = 0;
    int multiplier_ = 1;
    int power_bonus_ = 0;
};

/**
 * Shuffle function declarations
 * outShuffle: Shuffle the discard pile into the draw pile in order
 * inShuffle: Shuffle the discard pile into the draw pile in reverse order
 * oddEvenShuffle: Separate the discard pile by odd and even positions, with odd positions first and even positions last
 * Please implement these functions in the later part of this file
*/
void outShuffle(GameState &game_state);
void inShuffle(GameState &game_state);
void oddEvenShuffle(GameState &game_state);

/**
 * Game controller class
 * Used to handle game logic
 */
class GameController {
private:
    GameState game_state_;
    // Shuffle function pointer
    void (*shuffle_)(GameState&);
public:
    explicit GameController(int mode);

    // Draw 5 cards from the draw pile to the hand pile
    // This is a function declaration, please implement this function in the later part of the file
    void draw();

    // Play the card_to_play-th card in the hand pile (counting from 1)
    // This is a function declaration, please implement this function in the later part of the file
    void play(int card_to_play);

    // End the current turn, move all cards in the hand pile to the discard pile
    // This is a function declaration, please implement this function in the later part of the file
    void finish();

    // Shuffle in the specified manner
    // This is a function declaration, please implement this function in the later part of the file
    void shuffle();

    // Query current total score
    // This is a function declaration, please implement this function in the later part of the file
    int queryScore();

    // Query draw pile size
    int queryDrawPileSize() const {
        return game_state_.draw_pile_.size();
    }

    // Query hand pile size
    int queryHandSize() const {
        return game_state_.hand_.size();
    }

    // Query discard pile size
    int queryDiscardPileSize() const {
        return game_state_.discard_pile_.size();
    }

    // Get reference to the draw pile
    // Used to load cards in main.cpp
    Pile& drawPile() {
        return game_state_.draw_pile_;
    }
};

// ================= 洗牌函数实现 ===================

/**
 * Shuffle the discard pile into the draw pile in order
 * TODO: Implement this function
 */
void outShuffle(GameState& game_state) {
    // Move all cards from discard pile to draw pile tail, maintaining order
    while (!game_state.discard_pile_.empty()) {
        Card* card = game_state.discard_pile_.removeFromHead();
        game_state.draw_pile_.addToTail(card);
    }
}

/**
 * Shuffle the discard pile into the draw pile in reverse order
 * TODO: Implement this function
 */
void inShuffle(GameState& game_state) {
    // Move all cards from discard pile to draw pile tail, reversing order
    while (!game_state.discard_pile_.empty()) {
        Card* card = game_state.discard_pile_.removeFromTail();
        game_state.draw_pile_.addToTail(card);
    }
}

/**
 * Separate the discard pile by odd and even positions, with odd positions first and even positions last, and shuffle into the draw pile
 * TODO: Implement this function
 */
void oddEvenShuffle(GameState& game_state) {
    // Collect all cards from discard pile
    Card* cards[10000]; // Temporary array to hold cards
    int count = 0;
    while (!game_state.discard_pile_.empty()) {
        cards[count++] = game_state.discard_pile_.removeFromHead();
    }

    // First add odd indexed cards (1, 3, 5, ...)
    for (int i = 0; i < count; i += 2) {
        game_state.draw_pile_.addToTail(cards[i]);
    }

    // Then add even indexed cards (2, 4, 6, ...)
    for (int i = 1; i < count; i += 2) {
        game_state.draw_pile_.addToTail(cards[i]);
    }
}

// ======================================================




// ================== Game State Structure Implementation ===================
// If you added functions to the GameState structure, you can implement them here





// ======================================================




// ================= Card Class Implementation ===========================

// === Score Card Class Implementation ===
ScoreCard::ScoreCard(int point) : point_(point) {
}

void ScoreCard::play(GameState &game_state) {
    // Final score = (card base score + accumulated power bonus) × current multiplier
    int score = (point_ + game_state.power_bonus_) * game_state.multiplier_;
    game_state.total_score_ += score;
    // Reset multiplier to 1 after using score card
    game_state.multiplier_ = 1;
}
// ===================

// === Skill Card Class Implementation ===
SkillCard::SkillCard(SkillCardType skill_card_type) : skill_card_type_(skill_card_type) {
}

void SkillCard::play(GameState &game_state) {
    if (skill_card_type_ == MULTIPLIER) {
        // Increase multiplier by 1
        game_state.multiplier_++;
    } else if (skill_card_type_ == HEAD_BUTT) {
        // Move the tail card of discard pile to head of draw pile
        if (!game_state.discard_pile_.empty()) {
            Card* card = game_state.discard_pile_.removeFromTail();
            game_state.draw_pile_.addToHead(card);
        }
    }
}
// ===================

// === Power Card Class Implementation ===
PowerCard::PowerCard(int plus_count) : plus_count_(plus_count) {
}

void PowerCard::play(GameState &game_state) {
    // Permanently increase power bonus
    game_state.power_bonus_ += plus_count_;
}
// ===================

// ======================================================




// ================= Pile Class Implementation ===========================
Pile::Pile() : size_(0), head_(nullptr), tail_(nullptr) {
}

Pile::~Pile() {
    // Delete all nodes and cards
    Node* current = head_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current->card;
        delete current;
        current = next;
    }
}

/**
  * Add a card to the end of the pile
  * TODO: Implement this function
  */
void Pile::appendCard(Card* card) {
    addToTail(card);
}

Card* Pile::removeFromHead() {
    if (empty()) return nullptr;

    Node* node = head_;
    Card* card = node->card;
    head_ = head_->next;
    if (head_ == nullptr) {
        tail_ = nullptr;
    }
    delete node;
    size_--;
    return card;
}

void Pile::addToTail(Card* card) {
    Node* node = new Node(card);
    if (empty()) {
        head_ = tail_ = node;
    } else {
        tail_->next = node;
        tail_ = node;
    }
    size_++;
}

Card* Pile::removeFromTail() {
    if (empty()) return nullptr;

    if (head_ == tail_) {
        // Only one node
        Card* card = head_->card;
        delete head_;
        head_ = tail_ = nullptr;
        size_--;
        return card;
    }

    // Find second to last node
    Node* current = head_;
    while (current->next != tail_) {
        current = current->next;
    }

    Card* card = tail_->card;
    delete tail_;
    tail_ = current;
    tail_->next = nullptr;
    size_--;
    return card;
}

void Pile::addToHead(Card* card) {
    Node* node = new Node(card);
    if (empty()) {
        head_ = tail_ = node;
    } else {
        node->next = head_;
        head_ = node;
    }
    size_++;
}

Card* Pile::removeAt(int index) {
    if (index < 1 || index > size_) return nullptr;

    if (index == 1) {
        return removeFromHead();
    }

    // Find the node before the one to remove
    Node* current = head_;
    for (int i = 1; i < index - 1; i++) {
        current = current->next;
    }

    Node* to_remove = current->next;
    Card* card = to_remove->card;
    current->next = to_remove->next;

    if (to_remove == tail_) {
        tail_ = current;
    }

    delete to_remove;
    size_--;
    return card;
}

// ======================================================




// ================= Game Controller Class Implementation ======================

/**
 * Constructor, initialize game state and shuffle mode
 * @param mode Shuffle mode, 1 for outShuffle, 2 for inShuffle, 3 for oddEvenShuffle
 * TODO: Implement this function
 */
GameController::GameController(int mode){
    if (mode == 1) {
        shuffle_ = outShuffle;
    } else if (mode == 2) {
        shuffle_ = inShuffle;
    } else if (mode == 3) {
        shuffle_ = oddEvenShuffle;
    }
}

/**
 * Draw 5 cards from the draw pile to the hand pile
 * TODO: Implement this function
 */
void GameController::draw() {
    for (int i = 0; i < 5; i++) {
        // Check if draw pile is empty during drawing
        if (game_state_.draw_pile_.empty()) {
            // Trigger shuffle if discard pile is not empty
            if (!game_state_.discard_pile_.empty()) {
                shuffle();
            }
        }

        // Try to draw a card
        if (!game_state_.draw_pile_.empty()) {
            Card* card = game_state_.draw_pile_.removeFromHead();
            game_state_.hand_.addToTail(card);
        }
    }
}

/**
 * Play the card_to_play-th card in the hand pile (counting from 1)
 * TODO: Implement this function
 * Reference implementation approach:
    * 1. Find the corresponding hand card node
    * 2. Remove the card from the hand pile
    * 3. Execute the card's effect
    * 4. If it's a score card or skill card, move it to the discard pile; if it's a power card, delete it
 */
void GameController::play(int card_to_play) {
    // Remove card from hand
    Card* card = game_state_.hand_.removeAt(card_to_play);

    if (card == nullptr) return;

    // Execute card effect
    card->play(game_state_);

    // Check if it's a power card
    PowerCard* power_card = dynamic_cast<PowerCard*>(card);
    if (power_card != nullptr) {
        // Power card is destroyed
        delete card;
    } else {
        // Score card or skill card goes to discard pile
        game_state_.discard_pile_.addToTail(card);
    }
}

/**
 * Shuffle in the specified manner
 * TODO: Implement this function
 */
void GameController::shuffle() {
    shuffle_(game_state_);
}

/**
 * End the current turn, move all cards in the hand pile to the discard pile
 * TODO: Implement this function
 */
void GameController::finish() {
    while (!game_state_.hand_.empty()) {
        Card* card = game_state_.hand_.removeFromHead();
        game_state_.discard_pile_.addToTail(card);
    }
}

/**
 * Query current total score
 * TODO: Implement this function
 */
int GameController::queryScore() {
    return game_state_.total_score_;
}

#endif //GAME_H
