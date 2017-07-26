import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        
        self.positives = set()
        file1 = open(positives, "r")
        for line in file1:
            if line.startswith(';') == False:
                self.positives.add(line.rstrip("\n"))
        file1.close()
        
        self.negatives = set()
        file2 = open(negatives, "r")
        for line in file2:
            if line.startswith(';') == False:
                self.negatives.add(line.rstrip("\n"))
        file2.close()
        
    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        for word in tokens:
            if word.lower() in self.positives:
                score += 1
            elif word.lower() in self.negatives:
                score -= 1
            else:
                continue
                
        return score

        return 0
