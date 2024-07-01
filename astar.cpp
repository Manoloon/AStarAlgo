void Enemy::UpdatePathFinding(Level &level, sf::Vector2f playerPosition)
{
    //vars
    std::vector<Tile*> openList;
    std::vector<Tile*> closeList;
    std::vector<Tile*> pathList;
    std::vector<Tile*>::iterator position;
    Tile* currentNode;


    //reset node
    level.ResetNodes();
    m_targetPositions.clear();

    //store the start position node and the goal nodes
    Tile* startNode = level.GetTile(TransformComp->GetPosition());
    Tile* GoalNode = level.GetTile(playerPosition);

    //Check we have a valid path to find. If not we just end function.
    if(startNode == GoalNode)
    {
        m_targetPositions.clear();
        return;
    }
    // Pre-compute H cost (estimated cost to goal)
    for(int i=0;i<level.GetSize().x;++i)
    {
        for(int j=0;j<level.GetSize().y;++j)
        {
            int rowOffset,  heightOffset;
            Tile* node = level.GetTile(i,j);
            heightOffset = abs(node->rowIndex - GoalNode->rowIndex);
            rowOffset = abs(node->columnIndex - GoalNode->columnIndex);
            node->H = heightOffset + rowOffset;
        }
    }
    // Add the start node to the openList.
    openList.push_back(startNode);
    //while we have values to check in the openList.
    while(!openList.empty())
    {
        //Find node in the openList with the lowest F value and mark it as current.
        int lowestF = INT_MAX;
        for(Tile* tile : openList)
        {
            if(tile->F < lowestF)
            {
                lowestF = tile->F;
                currentNode = tile;
            }
        }
        //Remove current node from openList and add it to closeList
        position = std::find(openList.begin(),openList.end(),currentNode);
        if(position != openList.end())
        {
            openList.erase(position);
        }
        closeList.push_back(currentNode);
    }
    // Find all valid adjacent nodes.
    std::vector<Tile*> adjacentTiles;
    Tile* node;

    //NO DIAGONAL MOVEMENT ALLOW
    // Manhattan Distance not Allowed DIAGONAL MOVEMENT
    //Eucledian Distance allows it : Sqrt((loc.x -loc2.x)^2 +(loc.y - loc2.y)^2)
    //start with the one in front.
    node = level.GetTile(currentNode->columnIndex,currentNode->rowIndex -1);
    if((node !=nullptr) && (level.IsFloor(*node)))
    {
        adjacentTiles.push_back(level.GetTile(currentNode->columnIndex, currentNode->rowIndex -1));
    }
    // Right node
    node = level.GetTile(currentNode->columnIndex+1,currentNode->rowIndex);
    if((node !=nullptr) && (level.IsFloor(*node)))
    {
        adjacentTiles.push_back(level.GetTile(currentNode->columnIndex+1, currentNode->rowIndex));
    }
    // Bottom node
    node = level.GetTile(currentNode->columnIndex,currentNode->rowIndex+1);
    if((node !=nullptr) && (level.IsFloor(*node)))
    {
        adjacentTiles.push_back(level.GetTile(currentNode->columnIndex, currentNode->rowIndex+1));
    }
    //Left node
    node = level.GetTile(currentNode->columnIndex-1,currentNode->rowIndex);
    if((node !=nullptr) && (level.IsFloor(*node)))
    {
        adjacentTiles.push_back(level.GetTile(currentNode->columnIndex-1, currentNode->rowIndex));
    }

    //For all adjacent nodes. We check if the node is the goal node ,because if it so,
    // that means that this is the best lowest F path.
    for(Tile* locNode : adjacentTiles)
    {
        if(locNode == GoalNode)
        {
            locNode->parentNode = currentNode;
            //store the current path
            while(locNode->parentNode != nullptr)
            {
                pathList.push_back(locNode);
                locNode = locNode->parentNode;
            }
            // Empty the openList and break the loop.
            openList.clear();
            break;
        }
        else
        {
            //if the locNode is not in the closeList
            position = std::find(closeList.begin(), closeList.end(), locNode);
            if(position == closeList.end())
            {
                //if the locNode is not in the openList
                position = std::find(openList.begin(), openList.end(), locNode);
                if(position == openList.end())
                {
                    //add the locNode to the openList
                    openList.push_back(locNode);
                    //set the parent of the locNode to the currentNode
                    locNode->parentNode = currentNode;

                    //Calculate G (total movement cost so far) cost
                    locNode->G = currentNode->G + 10;
                    //calculate the F ( total movement cost + Heuristic) cost
                    locNode->F = locNode->G + locNode->H;
                }
                else
                {
                    // check if this path is quicker than the other.
                    int tempG = currentNode->G +10;
                    if(tempG < locNode->G)
                    {
                        //re-parent locNode to this one
                        locNode->parentNode = currentNode;
                    }
                }
            }

        }
    }
    //Store the node locations as the enemy target locations.
    for(Tile* tile:pathList)
    {
        m_targetPositions.push_back(level.GetActualTileLocation(tile->columnIndex,tile->rowIndex));
    }
    //Reverse the target position as we read them from goal to origin.
    std::reverse(m_targetPositions.begin(),m_targetPositions.end());
}