# killCalc

Calculates kills from a survival games lobby from a log file

Drop the log file onto the .exe

Prints out every killer and the winner. Winner is based on last kill so could be inaccurate.

# Notes

Supports up to 32 killers which means a lobby of 64 solo players.

If you want to self compile, just ```gcc main.c``` should do the trick.

Appreciating pull requests, code was done in a rush to "just work", needs cleanup in many places.
