module Sudoku
(
  Sudoku,
  Jigsaw,
  solve,
  isSolved,
  pPrint,
  squigglySudoku
) where

import Data.Maybe
import Data.List

type Sudoku = [Int]
type Jigsaw = [Int]

chunksOf :: Int -> [a] -> [[a]]
chunksOf _ [] = []
chunksOf n xs
  | n > 0     = take n xs : (chunksOf n $ drop n xs)
  | otherwise = []

--mapMaybe' :: (a -> b) -> Maybe a -> Maybe b
mapMaybe' _ [] _  = []
mapMaybe' f (x:xs) c = case (f x c) of
  Just y -> y : mapMaybe' f xs c
  Nothing -> mapMaybe' f xs c

solve :: Sudoku->Jigsaw-> Maybe Sudoku

solve sudoku jigsaw
  | isSolved sudoku jigsaw = Just sudoku
  | otherwise = do
    index <- elemIndex 0 sudoku
    let sudokus = [nextTest sudoku index i | i <- [1..9],
                                  checkRow (nextTest sudoku index i) index,
                                  checkColumn (nextTest sudoku index i) index,
                                  checkBox (nextTest sudoku index i) (jigsaw!!index)]

    listToMaybe $ mapMaybe' solve sudokus jigsaw

  where nextTest sudoku index i = take index sudoku ++ [i] ++ drop (index+1) sudoku
        checkRow sudoku index = (length $ getRow sudoku index) == (length $ nub $ getRow sudoku index)
        checkColumn sudoku index = (length $ getColumn sudoku index) == (length $ nub $ getColumn sudoku index)
        checkBox sudoku index = (length $ getBox sudoku index) == (length $ nub $ getBox sudoku index)
        getRow sudoku index = filter (/=0) $ (chunksOf 9 sudoku) !! (quot index 9)
        getColumn sudoku index = filter (/=0) $ (transpose $ chunksOf 9 sudoku) !! (mod index 9)
        getBox sudoku index = filter (/=0) $ [sudoku!!i | i<-(elemIndices (index) jigsaw)]


isSolved :: Sudoku ->Jigsaw-> Bool

isSolved sudoku jigsaw

  | product sudoku == 0 = False
  | map (length . nub) sudokuRows /= map length sudokuRows = False
  | map (length . nub) sudokuColumns /= map length sudokuColumns = False
  | map (length . nub) sudokuBoxes /= map length sudokuBoxes = False
  | otherwise = True

  where sudokuRows = chunksOf 9 sudoku
        sudokuColumns = transpose sudokuRows
        sudokuBoxes = [[sudoku!!i | i<-(elemIndices (index) jigsaw)] : [] | index <- [1..9]]



pPrint :: Sudoku -> String

pPrint sudoku = intercalate "\n" $ map (intercalate " " . map show) $ chunksOf 9 sudoku


squigglySudoku :: Sudoku->Jigsaw->IO()

squigglySudoku val blk = do
    putStrLn $ pPrint val ++ "\n\n"
    putStrLn $ pPrint $ fromMaybe [] $ solve val blk
