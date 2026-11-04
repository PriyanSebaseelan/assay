Nov 2: Set everything up
Nov 3: Read Crafting Interpreters and finished initScanner
Nov 4: Finished isAtEnd and advance

4. Write four small helper functions above scanToken. Each one is 1–3 lines:
Helper	What it does
isAtEnd()	true if *scanner.current is the '\0' at the end of the string
advance()	returns the current character and moves current forward by one
makeToken(type)	builds a Token whose text runs from start to current, with the current line
errorToken(message)	builds a TOKEN_ERROR whose start and length point at message

5. Rewrite scanToken so it does this, in order:

set scanner.start = scanner.current (a new token begins here)
if isAtEnd(), return makeToken(TOKEN_EOF)
char c = advance();
switch (c): one case for each of the 14 single characters, ( ) { } [ ] , . - + ; / * %, each returning makeToken(...) with the matching TOKEN_ type
after the switch: return errorToken("Unexpected character.");

6. Build and test:

sh
make test
./build/assay --tokens tests/scanner/punctuation.assay

test_single_char_tokens should now pass. punctuation will still fail, because spaces aren’t handled yet and they’ll show up as ERROR tokens. That’s your next step.

7. Skip the spaces. At the very top of scanToken, add a loop: while the next character is ' ', '\t' or '\r', call advance(). Run make test again, and punctuation should pass.

8. Commit:

sh
git add -A
git commit -m "scanner: single-character tokens and whitespace"
git push

9. Journal. Write two lines in JOURNAL.md: what you built, and one thing that surprised you.

Once you’ve tried it yourself

Read the rest of section 16.2 and compare it with your code. Your makeToken should look roughly like the book’s. If yours differs, work out which version is better and why.

If you get stuck
Golden test fails: read the diff. - lines are what was expected, + lines are what you produced.
Unit test fails: the message gives the exact line in test_scanner.c and both values.
Test “timed out”: you have an infinite loop, almost certainly a loop that never calls advance().
Still stuck after 30 minutes: send me the failing output and your code, and I’ll give you a hint.