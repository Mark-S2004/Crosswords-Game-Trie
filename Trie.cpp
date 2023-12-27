#include "trie.hpp"

Trie::Trie()
{
    root = new Node();
}

Trie::~Trie()
{
    deleteTrie(root);
}

bool Trie::empty() const
{
    return root->isLeaf();
}

void Trie::deleteTrie(nodePointer node)
{
    nodePointer childNode;
    for (int i = 0; i < alphabet_size; i++)
    {
        childNode = node->children[i];
        if (childNode)
        {
            if (childNode->isLeaf())
                delete childNode;
            else
                deleteTrie(childNode);
        }
    }
    delete node;
}

void Trie::copyConstructor(nodePointer thisPtr, nodePointer origPtr)
{
    for (int i = 0; i < alphabet_size; i++)
    {
        if (origPtr->children[i])
        {
            thisPtr->children[i] = new Node(origPtr->children[i]->isEndofWord);
            copyConstructor(thisPtr->children[i], origPtr->children[i]);
        }
    }
}

Trie::Trie(const Trie &origTrie) : root(new Node())
{
    copyConstructor(root, origTrie.root);
}

const Trie &Trie::operator=(const Trie &rhs)
{
    if (this != &rhs)
    {
        this->~Trie();
        root = new Node();
        copyConstructor(root, rhs.root);
    }
    return *this;
}

void Trie::insert(const string word)
{
    Trie::nodePointer current = root;
    for (char c : word)
    {
        int i = tolower(c) - 'a';
        if (current->children[i] == nullptr)
        {
            current->children[i] = new Node();
        }
        current = current->children[i];
    }
    current->isEndofWord = true;
}

bool Trie::search(const string word)
{
    nodePointer ptr = root;
    for (char c : word)
    {
        if (ptr->children[tolower(c) - 'a'] == NULL)
        {
            return false;
        }
        ptr = ptr->children[tolower(c) - 'a'];
    }
    return ptr->isEndofWord;
}

void Trie::deleteAllNodes(nodePointer lastPrefixNode, short lastPrefixCharIndex, nodePointer lastNode, string word)
{
    nodePointer prevPtr = lastPrefixNode, ptr = lastPrefixNode->children[word[lastPrefixCharIndex] - 'a'];
    short ptrIndex = lastPrefixCharIndex;
    if (lastNode == lastPrefixNode)
        return;
    while (ptr != lastNode)
    {
        prevPtr = ptr;
        ptr = prevPtr->children[word[++ptrIndex] - 'a'];
    }
    delete ptr;
    prevPtr->children[word[ptrIndex] - 'a'] = NULL;
    deleteAllNodes(lastPrefixNode, lastPrefixCharIndex, prevPtr, word);
}

void Trie::deleteWord(string word)
{
    for (int k = 0; k < word.length(); k++)
        word[k] = tolower(word[k]);

    nodePointer ptr = root, lastPrefixNode = NULL;
    short i, j, count, lastPrefixCharIndex = (word[0] - 'a');

    for (j = 0; j < word.length(); j++)
    {
        if (!ptr->children[word[j] - 'a'])
        {
            return;
        }
        count = 0;
        for (i = 0; i < alphabet_size; i++)
            if (ptr->children[i])
                count++;
        if (count > 1 || ptr->isEndofWord)
        {
            lastPrefixNode = ptr;
            lastPrefixCharIndex = j;
        }
        ptr = ptr->children[word[j] - 'a'];
    }

    if (!ptr->isLeaf())
        ptr->isEndofWord = false;
    else
        deleteAllNodes(lastPrefixNode, lastPrefixCharIndex, ptr, word);
}

void Trie::displayReq(ostream &out) const
{
    return display(out, root, "");
}

void Trie::display(ostream &out, nodePointer node, string str) const
{
    string str2;
    if (node->isEndofWord)
    {
        str += '\0';
        out << str << endl;
    }
    for (int i = 0; i < alphabet_size; i++)
    {
        if (node == root)
            str = "";
        if (node->children[i])
        {
            str2 = str;
            str2 += i + 'a';
            display(out, node->children[i], str2);
        }
    }
}

void Trie::toPque(myPQ &words) const
{
    toPqueAux(root, "", words);
}

void Trie::toPqueAux(nodePointer node, string str, myPQ &words) const
{
    string str2;
    if (node->isEndofWord)
    {
        str += '\0';
        words.push(str);
    }
    for (int i = 0; i < alphabet_size; i++)
    {
        if (node == root)
            str = "";
        if (node->children[i])
        {
            str2 = str;
            str2 += i + 'a';
            toPqueAux(node->children[i], str2, words);
        }
    }
}

void Trie::toCrosswordsBoard()
{
    myPQ words;

    this->toPque(words);
    int maxLength = words.top().length() - 1;
    crosswordBoard.resize(maxLength * 3, vector<pair<char, bool>>(maxLength * 3, {'_', false}));

    for (int i = 0; i < maxLength; ++i)
        crosswordBoard[maxLength + i][maxLength].first = words.top()[i];
    words.pop();

    while (words.size())
    {
        placeWordOnGrid(words.top(), maxLength);
        words.pop();
    }
}

void Trie::placeWordOnGrid(string word, int maxLength)
{
    int i, j, k;
    bool placed = false;
    for (i = 0; i < maxLength; ++i)
    {
        for (j = 0; j < word.length() - 1; ++j)
        {
            if (crosswordBoard[maxLength + i][maxLength].first == word[j] && !crosswordBoard[maxLength + i][maxLength].second)
            {
                for (k = j + 1; k < word.length() - 1; ++k)
                    crosswordBoard[maxLength + i][maxLength + (k - j)].first = word[k];
                if (j)
                    for (k = j - 1; k >= 0; --k)
                        crosswordBoard[maxLength + i][maxLength - (j - k)].first = word[k];
                crosswordBoard[maxLength + i][maxLength].second = true;
                placed = true;
                break;
            }
        }
        if (placed)
            break;
    }
    if (!placed)
    {
        for (i = 0; i < word.length() - 1; ++i)
        {
            crosswordBoard[maxLength * 2][maxLength + i].first = word[i];
        }
    }
}

ostream &operator<<(ostream &out, const Trie &aTrie)
{
    aTrie.displayReq(out);
    return out;
}

istream &operator>>(istream &in, Trie &trie)
{
    string word;
    in >> word;
    trie.insert(word);
    return in;
}
