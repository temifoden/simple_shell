## ALGORITHM OF THE SHELL
main
    if ac > 1
        file execution
    else if !isatty
        pipe execution
    else
        while true:
            display prompt: ($)
            getline stdin
            tokenize line
            if !tokens
            continue
            execute

## _stdlib todo
# Temitayo
_strncat
strcmp
_strlen

# peter
getenv
strtok

write atoi
write isnum
