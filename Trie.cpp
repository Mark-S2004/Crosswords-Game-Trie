#include "trie.hpp"

bool Trie::search(const string word)
{
    nodePointer ptr = root;
    for (char c : word)
    {
        if (ptr->children[c - 'a'] == NULL)
        {
            return false;
        }
        ptr = ptr->children[c - 'a'];
    }
    return ptr->isEndofWord;
}

void Trie::deleteAllNodes(nodePointer lastPrefixNode, short lastPrefixCharIndex, nodePointer lastNode, string word)
{
    nodePointer prevPtr = lastPrefixNode, ptr = lastPrefixNode->children[word[lastPrefixCharIndex] - 'a'];
    while (ptr != lastNode)
    {
        prevPtr = ptr;
        ptr = prevPtr->children[word[++lastPrefixCharIndex] - 'a'];
    }
    delete ptr;
    prevPtr->children[word[lastPrefixCharIndex] - 'a'] = NULL;
    deleteAllNodes(lastPrefixNode, lastPrefixCharIndex, prevPtr, word);
}

void Trie::deleteWord(const string word)
{
    nodePointer ptr = root, lastPrefixNode = NULL;
    short i, j, count = 0, lastPrefixCharIndex = (word[0] - 'a');

    for (j = 0; j < word.length(); j++)
    {
        if (!ptr->children[word[j] - 'a'])
        {
            return;
        }
        for (i = 0; i < 26; i++)
            if (ptr->children[i])
                count++;
        if (count > 1)
        {
            lastPrefixNode = ptr;
            lastPrefixCharIndex = j;
        }
        ptr = ptr->children[word[j] - 'a'];
    }

    count = 0;
    for (i = 0; i < 26; i++)
    {
        if (ptr->children[i])
        {
            count++;
            break;
        }
    }

    if (count)
        ptr->isEndofWord = false;
    else if (lastPrefixNode)
        deleteAllNodes(lastPrefixNode, lastPrefixCharIndex, ptr, word);
    else
        deleteAllNodes(root, lastPrefixCharIndex, ptr, word);
}