flipCoin' :: String -> Integer -> [Integer]

flipCoin' [] n = [0]

flipCoin' [x] n
    | x == 'H' = [0]
    | x == 'T' = n : flipCoin' [] n

flipCoin' (x:xs) n
    | x == y = flipCoin' (xs) (n+1)
    | otherwise = n : flipCoin' (xs) (n+1)
    where y = head xs

flipCoin :: String->[Integer]

flipCoin x = flipCoin' x 1
