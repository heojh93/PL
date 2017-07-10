module PRIME where

  prime m n = take n (filter isPrime [m..])
  
  isPrime c = not (any (\k -> (c `mod` k) == 0) [2..(c `div` 2)])
